#include <stdio.h>
#include <string.h>
#include "cart.h"

#define CART_FILE "carts.dat"


Cart initializeCart(const char *userId) {
    Cart cart = {{0}, {}, 0};
    strncpy(cart.userId, userId, sizeof(cart.userId) - 1);
    cart.userId[sizeof(cart.userId) - 1] = '\0';

    if (loadCartFromFile(userId, &cart)) {
        printf("Loaded existing cart for User ID: %s\n", userId);
    } else {
        printf("No existing cart found. Creating a new cart for User ID: %s\n", userId);
    }
    return cart;
}


void addProductToCart(Cart *cart, Product product, int quantity) {
    for (int i = 0; i < cart->itemCount; i++) {
        if (cart->items[i].product.id == product.id) {
            cart->items[i].quantity += quantity;
            cart->items[i].totalCost = cart->items[i].quantity * cart->items[i].product.price;
            printf("Updated '%s' quantity to %d. Total cost: $%.2f\n",
                   product.name, cart->items[i].quantity, cart->items[i].totalCost);
            return;
        }
    }

    if (cart->itemCount < 100) {
        cart->items[cart->itemCount].product = product;
        cart->items[cart->itemCount].quantity = quantity;
        cart->items[cart->itemCount].totalCost = quantity * product.price;
        printf("Added '%s' to cart with quantity %d. Total cost: $%.2f\n",
               product.name, quantity, cart->items[cart->itemCount].totalCost);
        cart->itemCount++;
    } else {
        printf("Cart is full!\n");
    }
    saveCartToFile(*cart);

}


float calculateCartTotal(Cart cart) {
    float total = 0.0;
    for (int i = 0; i < cart.itemCount; i++) {
        total += cart.items[i].totalCost;
    }
    return total;
}

void displayCart(Cart cart) {
    for (int i = 0; i < cart.itemCount; i++) {
        printf("%d - %s : $%.2f x %d = $%.2f\n",
               cart.items[i].product.id,
               cart.items[i].product.name,
               cart.items[i].product.price,
               cart.items[i].quantity,
               cart.items[i].totalCost);
    }
    printf("Total Cost of All Items: $%.2f\n", calculateCartTotal(cart));
}


void saveCartToFile(Cart cart) {
    FILE *file = fopen(CART_FILE, "rb");
    if (!file) {
        printf("Error: Could not open file to save cart.\n");
        return;
    }

    FILE *tempFile = fopen("temp.dat", "wb");
    if (!tempFile) {
        printf("Error: Could not create temporary file.\n");
        fclose(file);
        return;
    }

    Cart temp;
    int found = 0;

    while (fread(&temp, sizeof(Cart), 1, file)) {
        if (strcmp(temp.userId, cart.userId) == 0) {
            fwrite(&cart, sizeof(Cart), 1, tempFile);
            found = 1;
        } else {
            fwrite(&temp, sizeof(Cart), 1, tempFile);
        }
    }

    if (!found) {
        fwrite(&cart, sizeof(Cart), 1, tempFile);
    }

    fclose(file);
    fclose(tempFile);


    remove(CART_FILE);
    rename("temp.dat", CART_FILE);
}


int loadCartFromFile(const char *userId, Cart *cart) {
    FILE *file = fopen(CART_FILE, "rb");
    if (!file) {
        return 0;
    }

    Cart temp;
    while (fread(&temp, sizeof(Cart), 1, file)) {
        if (strcmp(temp.userId, userId) == 0) { // Compare userId strings
            *cart = temp;
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}


 float applyVoucher(float total, Voucher voucher) {
    float discount = total * (voucher.discount / 100);
    return total - discount;
}


void removeProductFromCart(Cart *cart, int productId) {
    int found = 0;

    // Find the product in the cart
    for (int i = 0; i < cart->itemCount; i++) {
        if (cart->items[i].product.id == productId) {
            found = 1;

            // Shift all subsequent items to fill the gap
            for (int j = i; j < cart->itemCount - 1; j++) {
                cart->items[j] = cart->items[j + 1];
            }

            cart->itemCount--; // Decrease the item count
            printf("Removed product with ID %d from the cart.\n", productId);
            break;
        }
    }

    if (!found) {
        printf("Product with ID %d not found in the cart.\n", productId);
    }

    // Save the updated cart back to the file
    saveCartToFile(*cart);
}

void editProductQuantity(Cart *cart, int productId, int newQuantity) {
    int found = 0;


    for (int i = 0; i < cart->itemCount; i++) {
        if (cart->items[i].product.id == productId) {
            found = 1;

            if (newQuantity <= 0) {

                printf("Quantity set to 0. Removing product '%s' from the cart.\n",
                       cart->items[i].product.name);

                for (int j = i; j < cart->itemCount - 1; j++) {
                    cart->items[j] = cart->items[j + 1];
                }

                cart->itemCount--;
            } else {

                cart->items[i].quantity = newQuantity;
                cart->items[i].totalCost = newQuantity * cart->items[i].product.price;

                printf("Updated product '%s' to new quantity: %d. Total cost: $%.2f\n",
                       cart->items[i].product.name,
                       newQuantity,
                       cart->items[i].totalCost);
            }
            break;
        }
    }

    if (!found) {
        printf("Product with ID %d not found in the cart.\n", productId);
    }

    saveCartToFile(*cart);
}
