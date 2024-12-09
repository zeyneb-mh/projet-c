#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>  // For date and time functions

#include "order.h"

#define ORDER_FILE "orders.dat"

// Create an Order





Order createOrder(Cart *cart, DeliveryType deliveryType, const char *promoCode, const char *userId) {
    Order order;
    order.cart = *cart;  // Copy the entire cart content into the order
    order.deliveryType = deliveryType;

    // Get the current date and time
    time_t now = time(NULL);
    order.orderDate = *localtime(&now);

    loadProducts();


        // Step 1: Check if the products in the cart are available in stock
    for (int i = 0; i < cart->itemCount; i++) {
        int productId = cart->items[i].product.id;
        int requestedQuantity = cart->items[i].quantity;

        Product product = getProductById(productId);

        // Check if the product is in stock
        if (product.stock < requestedQuantity) {
            // Product is out of stock or requested quantity exceeds stock
            printf("Error: Not enough stock for product '%s'. Requested: %d, Available: %d\n",
                   product.name, requestedQuantity, product.stock);
            // Abort the order process
            return order;
            break;
        }
    }

    // Load vouchers from the file
    loadVouchers();

    // Copy the promo code into the order
    if (promoCode) {
        strncpy(order.promoCode, promoCode, sizeof(order.promoCode) - 1);
        order.promoCode[sizeof(order.promoCode) - 1] = '\0'; // Ensure null termination
    } else {
        order.promoCode[0] = '\0'; // Empty promo code
    }

    // Copy the user ID
    strncpy(order.userId, userId, sizeof(order.userId) - 1);
    order.userId[sizeof(order.userId) - 1] = '\0'; // Ensure null termination

    // Find and validate the promo code
    Voucher *voucher = NULL;
    if (strlen(promoCode) > 0) {
        voucher = findVoucherByCode(promoCode);
        if (voucher == NULL) {
            // Invalid promo code if not found
            printf("Invalid promo code '%s'.\n", promoCode);
            voucher = NULL;  // Invalid voucher
        } else {
            // Validate the voucher
            if (!validateVoucher(voucher)) {
                printf("Voucher '%s' is already used or invalid.\n", promoCode);
                voucher = NULL;  // Invalid voucher
            } else {
                printf("Voucher '%s' is valid and will be applied.\n", promoCode);
            }
        }
    }



    // Step 2: Calculate the final price with voucher if valid
    order.finalPrice = calculateFinalPrice(*cart, deliveryType, voucher);

    // Step 3: Mark voucher as used if valid
    if (voucher) {
        printf("Marking voucher '%s' as used.\n", promoCode);
        markVoucherUsed(voucher); // Mark voucher as used

        // Save updated vouchers back to file
        FILE *file = fopen(VOUCHER_FILE, "w");
        if (file) {
            for (int i = 0; i < voucherCount; i++) {
                fprintf(file, "%d\n%.2f\n%s\n%s\n%d\n", vouchers[i].id, vouchers[i].discount,
                        vouchers[i].description, vouchers[i].code, vouchers[i].state);
            }
            fclose(file);
            printf("Voucher list has been saved to the file.\n");
        } else {
            printf("Error: Unable to save updated vouchers to the file.\n");
        }
    } else {
        printf("No valid promo code applied.\n");
    }

    // Step 4: Process the order (decrease stock and save the order)
    // Update the stock of products after order is confirmed
    for (int i = 0; i < cart->itemCount; i++) {
        int productId = cart->items[i].product.id;
        int requestedQuantity = cart->items[i].quantity;

        // Update the stock of the product
        Product *product = &products[productId-1]; // Assuming productId directly maps to the index in the products array
        product->stock -= requestedQuantity; // Reduce stock by the ordered quantity

        // Save updated product back to file
        modifyProduct(productId,*product);
    }

    // Save the order to the file
    saveOrderToFile(order);
    printf("Order has been saved to the file.\n");

    // Clear the cart contents after the order is confirmed
    cart->itemCount = 0; // Reset the item count
    saveCartToFile(*cart);

    printf("Cart has been cleared after placing the order.\n");

    return order;
}

// Calculate the Final Price of an Order
float calculateFinalPrice(Cart cart, DeliveryType deliveryType, const Voucher *voucher) {
    float totalPrice = calculateCartTotal(cart);

    // Apply voucher discount if provided
    if (voucher) {
        totalPrice = applyVoucher(totalPrice, *voucher);
    }

    // Add delivery charges if home delivery is selected
    if (deliveryType == HOME_DELIVERY && totalPrice < 300) {
        totalPrice += 7.0; // Add $7 for home delivery if total is less than $300
    }

    return totalPrice;
}

// Display Order Details
void displayOrder(Order order) {
    printf("\nOrder Details:\n");
    displayCart(order.cart); // Display the cart contents

    // Display delivery type
    printf("Delivery Type: %s\n",
           order.deliveryType == HOME_DELIVERY ? "Home Delivery" : "Local Pickup");

    // Display promo code if applied
    if (strlen(order.promoCode) > 0) {
        printf("Promo Code Applied: %s\n", order.promoCode);
    }

    // Display final price
    printf("Final Price: $%.2f\n", order.finalPrice);

    // Display order date
    printf("Order Date: %04d-%02d-%02d %02d:%02d:%02d\n",
           order.orderDate.tm_year + 1900,
           order.orderDate.tm_mon + 1,
           order.orderDate.tm_mday,
           order.orderDate.tm_hour,
           order.orderDate.tm_min,
           order.orderDate.tm_sec);
}

// Save an Order to the File
void saveOrderToFile(Order order) {
    FILE *file = fopen(ORDER_FILE, "ab");
    if (!file) {
        printf("Error: Unable to open orders file for writing.\n");
        return;
    }

    fwrite(&order, sizeof(Order), 1, file);
    fclose(file);
}

// List All Orders
void listAllOrders() {
    FILE *file = fopen(ORDER_FILE, "rb");
    if (!file) {
        printf("No orders found.\n");
        return;
    }

    Order order;
    printf("\nAll Orders:\n");
    while (fread(&order, sizeof(Order), 1, file)) {
        displayOrder(order);
        printf("--------------------------\n");
    }

    fclose(file);
}

// List Orders by User ID
void listOrdersByUserId(const char *userId) {
    FILE *file = fopen(ORDER_FILE, "rb");
    if (!file) {
        printf("No orders found.\n");
        return;
    }

    Order order;
    int found = 0;
    printf("\nOrders for User ID: %s\n", userId);
    while (fread(&order, sizeof(Order), 1, file)) {
        if (strcmp(order.userId, userId) == 0) {
            displayOrder(order);
            printf("--------------------------\n");
            found = 1;
        }
    }

    if (!found) {
        printf("No orders found for User ID: %s\n", userId);
    }

    fclose(file);
}


void listOrdersByDateRange(struct tm startDate, struct tm endDate) {
    FILE *file = fopen(ORDER_FILE, "rb");
    if (!file) {
        printf("No orders found.\n");
        return;
    }

    Order order;
    int found = 0;
    printf("\nOrders within Date Range:\n");
    while (fread(&order, sizeof(Order), 1, file)) {
        if (difftime(mktime(&order.orderDate), mktime(&startDate)) >= 0 &&
            difftime(mktime(&endDate), mktime(&order.orderDate)) >= 0) {
            displayOrder(order);
            printf("--------------------------\n");
            found = 1;
        }
    }

    if (!found) {
        printf("No orders found within the specified date range.\n");
    }

    fclose(file);
}


void inputDate(struct tm *date, const char *prompt) {
    printf("%s (YYYY-MM-DD): ", prompt);
    char dateInput[11]; // Buffer for user input
    if (scanf("%10s", dateInput) == 1) {
        sscanf(dateInput, "%4d-%2d-%2d", &date->tm_year, &date->tm_mon, &date->tm_mday);
        date->tm_year -= 1900; // Adjust year for struct tm
        date->tm_mon -= 1;     // Adjust month for struct tm (0-based)
        date->tm_hour = 0;
        date->tm_min = 0;
        date->tm_sec = 0;
    } else {
        printf("Invalid input. Please enter a valid date in YYYY-MM-DD format.\n");
    }
}

void handleListOrdersByDateRange() {
    struct tm startDate = {0};
    struct tm endDate = {0};

    // Get the start date from the user
    inputDate(&startDate, "Enter the start date");

    // Get the end date from the user
    inputDate(&endDate, "Enter the end date");

    // Validate that the start date is before or equal to the end date
    if (difftime(mktime(&endDate), mktime(&startDate)) < 0) {
        printf("Error: Start date cannot be after the end date.\n");
        return;
    }

    // Call the function to list orders by date range
    listOrdersByDateRange(startDate, endDate);
}

