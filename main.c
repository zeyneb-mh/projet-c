#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cart.c"
#include "user.c"
#include "prod.c"
#include "voucher.c"
#include "order.c"


#define CART_FILE "carts.txt"


void displayMenu() {
    printf("\n===== Menu =====\n");
    printf("1. Signup\n");
    printf("2. Login\n");
    printf("3. Exit\n");
    printf("================\n");
}

int main() {
    int choice;
    int role;
    User loggedInUser;


    while(1){
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                signup(); // Inscription d'un nouvel utilisateur
                break;
                    }
            case 2:{
                role = login(&loggedInUser); // Connexion et récupération du rôle (0 pour user, 1 pour admin)

                if (role == 0) {
                    printf("Login failed. Invalid email or password.\n");
                    break;
                }
                else if(role==-2){
                    printf("file error");
                    break;

                } else if (role == 1)
                {
                    printf("Login successful! You are an administrator.\n");

                   // Menu options for managing products or vouchers
                int manageChoice;

                        do {
                            printf("\n===== Admin Management Menu =====\n");
                            printf("1. Manage Products\n");
                            printf("2. Manage Vouchers\n");
                            printf("3. Logout\n");
                            printf("===============================\n");
                            printf("Enter your choice: ");
                            scanf("%d", &manageChoice);

                            switch (manageChoice) {
                                case 1: { // Manage Products
                                    int productChoice;
                                    loadProducts();

                                    do {
                                        printf("\n===== Product Management Menu =====\n");
                                        printf("1. Add Product\n");
                                        printf("2. Modify Product\n");
                                        printf("3. Delete Product\n");
                                        printf("4. List Products\n");
                                        printf("5. Filter Products\n");
                                        printf("6. Back to Admin Menu\n");
                                        printf("===================================\n");
                                        printf("Enter your choice: ");
                                        scanf("%d", &productChoice);

                                        switch (productChoice) {
                                            case 1: {
                                                Product product;
                                                printf("Enter product name: ");
                                                getchar();  // Clear newline
                                                fgets(product.name, sizeof(product.name), stdin);
                                                product.name[strcspn(product.name, "\n")] = '\0'; // Remove newline

                                                printf("Enter product price: ");
                                                scanf("%f", &product.price);

                                                printf("Enter product stock: ");
                                                scanf("%d", &product.stock);

                                                addProduct(product); // Add product
                                                break;
                                            }
                                            case 2: {
                                                int id;
                                                printf("Enter product ID to modify: ");
                                                scanf("%d", &id);

                                                Product newProduct;
                                                printf("Enter new product name: ");
                                                getchar();  // Clear newline
                                                fgets(newProduct.name, sizeof(newProduct.name), stdin);
                                                newProduct.name[strcspn(newProduct.name, "\n")] = '\0'; // Remove newline

                                                printf("Enter new product price: ");
                                                scanf("%f", &newProduct.price);

                                                printf("Enter new product stock: ");
                                                scanf("%d", &newProduct.stock);

                                                modifyProduct(id, newProduct); // Modify product
                                                break;
                                            }
                                            case 3: {
                                                int id;
                                                printf("Enter product ID to delete: ");
                                                scanf("%d", &id);

                                                deleteProduct(id); // Delete product
                                                break;
                                            }
                                            case 4: {
                                                listProducts(); // List products
                                                break;
                                            }
                                            case 5: {
                                                filterProducts();
                                                break;
                                            }
                                            case 6: {
                                                printf("Returning to Admin Management Menu...\n");
                                                break;
                                            }
                                            default:
                                                printf("Invalid choice. Please try again.\n");
                                        }
                                    } while (productChoice != 6);
                                    break;
                                }
                                case 2: { // Manage Vouchers
                                    int voucherChoice;
                                    loadVouchers(); // Load vouchers at the beginning of the menu
                                    do {
                                        printf("\n===== Voucher Management Menu =====\n");
                                        printf("1. Add Voucher\n");
                                        printf("2. Modify Voucher\n");
                                        printf("3. Delete Voucher\n");
                                        printf("4. List Vouchers\n");
                                        printf("5. Back to Admin Menu\n");
                                        printf("===================================\n");
                                        printf("Enter your choice: ");
                                        scanf("%d", &voucherChoice);

                                        switch (voucherChoice) {
                                            case 1: { // Add Voucher
                                                Voucher voucher = {0};
                                                printf("Enter voucher description: ");
                                                getchar();  // Clear newline
                                                fgets(voucher.description, sizeof(voucher.description), stdin);
                                                voucher.description[strcspn(voucher.description, "\n")] = '\0'; // Remove newline

                                                printf("Enter voucher discount (percentage): ");
                                                while (scanf("%f", &voucher.discount) != 1 || voucher.discount <= 0 || voucher.discount > 100) {
                                                    printf("Invalid input. Enter a valid discount percentage (0-100): ");
                                                    getchar(); // Clear invalid input
                                                }

                                                addVoucher(voucher); // Add voucher
                                                loadVouchers();      // Reload vouchers to keep data consistent
                                                break;
                                            }

                                            case 2: { // Modify Voucher
                                                int id;
                                                printf("Enter voucher ID to modify: ");
                                                scanf("%d", &id);

                                                Voucher *existingVoucher = NULL;
                                                for (int i = 0; i < voucherCount; i++) {
                                                    if (vouchers[i].id == id) {
                                                        existingVoucher = &vouchers[i];
                                                        break;
                                                    }
                                                }

                                                if (existingVoucher) {
                                                    Voucher newVoucher = *existingVoucher; // Start with existing data
                                                    printf("Enter new voucher description: ");
                                                    getchar();  // Clear newline
                                                    fgets(newVoucher.description, sizeof(newVoucher.description), stdin);
                                                    newVoucher.description[strcspn(newVoucher.description, "\n")] = '\0'; // Remove newline

                                                    printf("Enter new voucher discount (percentage): ");
                                                    while (scanf("%f", &newVoucher.discount) != 1 || newVoucher.discount <= 0 || newVoucher.discount > 100) {
                                                        printf("Invalid input. Enter a valid discount percentage (0-100): ");
                                                        getchar(); // Clear invalid input
                                                    }

                                                    modifyVoucher(id, newVoucher); // Modify voucher
                                                    loadVouchers();                // Reload vouchers to keep data consistent
                                                } else {
                                                    printf("Voucher with ID %d not found.\n", id);
                                                }
                                                break;
                                            }

                                            case 3: { // Delete Voucher
                                                int id;
                                                printf("Enter voucher ID to delete: ");
                                                scanf("%d", &id);

                                                deleteVoucher(id); // Delete voucher
                                                loadVouchers();    // Reload vouchers to keep data consistent
                                                break;
                                            }

                                            case 4: { // List Vouchers
                                                listVouchers(); // List all vouchers
                                                break;
                                            }

                                            case 5: { // Back to Admin Menu
                                                printf("Returning to Admin Management Menu...\n");
                                                break;
                                            }

                                            default:
                                                printf("Invalid choice. Please try again.\n");
                                        }
                                    } while (voucherChoice != 5);
                                    break;
                                }

                                case 3: {
                                    printf("Logging out...\n");
                                    break;
                                }
                                default:
                                    printf("Invalid choice. Please try again.\n");
                            }
                        }while(manageChoice != 3);
                        break;

                        }
                else if (role == 2){
                    printf("Login successful! You are a Client.\n");
                    int manageChoice;

                    Cart cart = initializeCart(loggedInUser.email); // Initialize or load user's cart

                    do {
                        printf("\n===== Client Management Menu =====\n");
                        printf("1. Products\n");
                        printf("2. Cart\n");
                        printf("3. Orders History\n");
                        printf("4. Logout\n");
                        printf("==================================\n");
                        printf("Enter your choice: ");
                        scanf("%d", &manageChoice);

                        switch (manageChoice) {
                            case 1: { // Display products and allow adding to the cart
                                loadProducts();
                                listProducts(); // List all products
                                 int choiceP;
                        do{
                            printf("1. Add Product to Panier\n");
                            printf("2.Back to menu\n");
                            printf("=========================\n");
                            printf("enter your choice :");
                            scanf("%d",&choiceP);
                            switch(choiceP){
                            case 1:{
                                 int productId, quantity;
                                printf("\nEnter the Product ID to add to your cart (or 0 to cancel): ");
                                scanf("%d", &productId);

                                if (productId == 0) {
                                    printf("Returning to Client Management Menu...\n");
                                    break;
                                }

                                printf("Enter the quantity: ");
                                scanf("%d", &quantity);

                                // Retrieve the product and add it to the cart
                                Product product = getProductById(productId);
                                if (product.price != -1) { // Assuming getProductById returns a dummy product if not found
                                    addProductToCart(&cart, product, quantity);
                                    printf("Product added to cart successfully!\n");
                                } else {
                                    printf("Invalid Product ID. Try again.\n");
                                }

                                break;
                            }
                            case 2:{
                                printf("Backing ...\n");
                                break;
                            }
                            default:{
                             printf("Invalid choice. Please try again.\n");
                             }
                            }
                        }while (choiceP!=2);

                                break;
                            }

                            case 2: { // Consult cart content
                                printf("\n===== Your Cart, Mr/Ms %s =====\n", loggedInUser.fullname);

                                displayCart(cart); // Display cart content
                                    //userId = loggedInUser.email;
                                    int manageCartChoice;

                                    //Cart cart = initializeCart(userId); // Initialize or load user's cart

                                    do {
                                        printf("\n===== Client Management Menu =====\n");
                                        printf("1. delete Item From Cart\n");
                                        printf("2. edit Item Quantity\n");
                                        printf("3. Place Order\n");
                                        printf("4. back to menu\n");
                                        printf("==================================\n");
                                        printf("Enter your choice: ");
                                        scanf("%d", &manageCartChoice);

                                        switch (manageCartChoice) {
                                            case 1: { // Display products and allow deleting a product from the cart
                                                int productIdToDelete;

                                                // Display cart contents before deletion
                                                displayCart(cart);

                                                printf("Enter the Product ID to delete: ");
                                                scanf("%d", &productIdToDelete);

                                                // Call the removeProductFromCart function
                                                removeProductFromCart(&cart, productIdToDelete);

                                                // Display cart contents after deletion
                                                displayCart(cart);

                                                break;
                                            }

                                            case 2: { // Consult cart and allow editing quantity
                                                int productIdToEdit, newQuantity;

                                                // Display cart contents before editing
                                                printf("\nYour Cart:\n");
                                                displayCart(cart);

                                                printf("Enter the Product ID to edit: ");
                                                scanf("%d", &productIdToEdit);

                                                printf("Enter the new quantity (set to 0 to remove the item): ");
                                                scanf("%d", &newQuantity);

                                                // Call the editProductQuantity function
                                                editProductQuantity(&cart, productIdToEdit, newQuantity);

                                                // Display cart contents after editing
                                                printf("\nYour Cart:\n");
                                                displayCart(cart);

                                                break;
                                            }
                                            case 3: {
                                                // Place Order
                                                    if (cart.itemCount == 0) {
                                                        printf("Your cart is empty. Please add items before placing an order.\n");
                                                        return 1;  // Exit the program if the cart is empty
                                                    }

                                                    // Choose delivery type
                                                    int deliveryChoice;
                                                    printf("Select Delivery Type:\n");
                                                    printf("1. Home Delivery ($7 extra if total < $300)\n");
                                                    printf("2. Local Pickup\n");
                                                    printf("Enter your choice: ");
                                                    scanf("%d", &deliveryChoice);

                                                    DeliveryType deliveryType = (deliveryChoice == 1) ? HOME_DELIVERY : LOCAL_PICKUP;

                                                    // Enter promo code
                                                    char promoCode[21];

                                                    while(getchar() != '\n');  // This will consume any remaining characters in the input buffer

                                                    printf("Enter Promo Code (or press ENTER to skip): ");
                                                    fgets(promoCode, sizeof(promoCode), stdin);
                                                    promoCode[strcspn(promoCode, "\n")] = '\0';
                                                    // Create the order
                                                    Order order = createOrder(&cart, deliveryType, promoCode, loggedInUser.email);

                                                    // Display order details
                                                    displayOrder(order);

                                                    // Exit or allow the user to continue shopping
                                                    printf("\nOrder placed successfully! Thank you for your purchase.\n");
                                                    printf("\nYour Cart:\n");
                                                    displayCart(cart);

                                                break;
                                            }

                                            case 4: {
                                                printf("Back to menu...\n");
                                                break;
                                            }

                                            default:
                                                printf("Invalid choice. Please try again.\n");
                                        }
                    } while (manageCartChoice != 4);

                                break;
                            }



                            case 3: {
                                printf("\n===== Your Cart, Mr/Ms %s =====\n", loggedInUser.fullname);
                                listOrdersByUserId(loggedInUser.email);
                                int choiceHistory;

                                do {
                                    printf("\n--- Order Management ---\n");
                                    printf("1. List Orders by Date Range\n");
                                    printf("2. Exit\n");
                                    printf("Enter your choice: ");
                                    if (scanf("%d", &choiceHistory) != 1) {
                                        printf("Invalid input. Please enter a number.\n");
                                        break;
                                    }

                                    switch (choiceHistory) {
                                        case 1:
                                            handleListOrdersByDateRange();
                                            break;
                                        case 2:
                                            printf("Exiting...\n");
                                            break;
                                        default:
                                            printf("Invalid choice. Please try again.\n");
                                            break;
                                    }
                                } while (choiceHistory != 2);
                                break;
                            }

                            case 4: {
                                printf("Logging out...\n");
                                break;
                            }

                            default:
                                printf("Invalid choice. Please try again.\n");
                        }
                    } while (manageChoice != 4);

                    break;
                    }

            }
            case 3: {
                printf("Exiting program...\n");
                exit(0);
            }
            default: {
                printf("Invalid choice. Please try again.\n");
                break;
            }
    }

    }

    return 0;
}

