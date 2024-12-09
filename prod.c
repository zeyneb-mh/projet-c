#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <float.h> // Include this for FLT_MAX


#include "prod.h"

#define PRODUCT_FILE "products.txt"

Product products[MAX_PRODUCTS];
int productCount = 0;

// Load products from the file
void loadProducts() {
    FILE* file = fopen(PRODUCT_FILE, "r");
    if (file == NULL) {
        // If file doesn't exist, initialize with empty list
        productCount = 0;
        return;
    }

    productCount = 0;

    while (fscanf(file, "%d\n%99[^\n]\n%f\n%d\n",
                  &products[productCount].id,
                  products[productCount].name,
                  &products[productCount].price,
                  &products[productCount].stock) != EOF) {
        productCount++;
    }

    fclose(file);
}

// Save a single product to the file (appending)
void saveProductToFile(Product product) {
    FILE* file = fopen(PRODUCT_FILE, "a");
    if (file == NULL) {
        printf("Error: Unable to open products file for writing.\n");
        return;
    }

    fprintf(file, "%d\n%s\n%.2f\n%d\n", product.id, product.name, product.price, product.stock);
    fclose(file);
}

// Save all products to the file (overwriting)
void saveAllProducts() {
    FILE* file = fopen(PRODUCT_FILE, "w");
    if (file == NULL) {
        printf("Error: Unable to open products file for saving.\n");
        return;
    }

    for (int i = 0; i < productCount; i++) {
        fprintf(file, "%d\n%s\n%.2f\n%d\n", products[i].id, products[i].name, products[i].price, products[i].stock);
    }

    fclose(file);
}

// Add a new product
void addProduct(Product product) {
    if (productCount >= MAX_PRODUCTS) {
        printf("Error: Maximum number of products reached.\n");
        return;
    }

    product.id = productCount + 1;
    products[productCount] = product;
    productCount++;

    saveProductToFile(product);
    printf("Product added successfully!\n");
}

// Modify an existing product by ID
void modifyProduct(int id, Product newProduct) {
    for (int i = 0; i < productCount; i++) {
        if (products[i].id == id) {
            newProduct.id = id; // Preserve the ID
            products[i] = newProduct;

            saveAllProducts();
            printf("Product modified successfully!\n");
            return;
        }
    }

    printf("Error: Product with ID %d not found.\n", id);
}

// Delete a product by ID
void deleteProduct(int id) {
    int found = 0;

    for (int i = 0; i < productCount; i++) {
        if (products[i].id == id) {
            found = 1;

            // Shift all products after the one being deleted
            for (int j = i; j < productCount - 1; j++) {
                products[j] = products[j + 1];
                products[j].id--; // Decrease the ID of each shifted product
            }

            productCount--;
            saveAllProducts();
            printf("Product with ID %d deleted successfully!\n", id);
            return;
        }
    }

    if (!found) {
        printf("Error: Product with ID %d not found.\n", id);
    }
}

// List all products
void listProducts() {
    if (productCount == 0) {
        printf("No products available.\n");
        return;
    }

    printf("ID | Name          | Price   | Stock\n");
    printf("-----------------------------------\n");
    for (int i = 0; i < productCount; i++) {
        printf("%d  | %-12s | %.2f $ | %d\n", products[i].id, products[i].name, products[i].price, products[i].stock);
    }
}

void filterProducts() {
    int choice;
    char searchText[100] = "";
    float minPrice = 0, maxPrice = FLT_MAX;

    printf("\nChoose filtering method:\n");
    printf("1. Filter by name\n");
    printf("2. Filter by price range\n");
    printf("3. Filter by both name and price range\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("Enter text to search in product names: ");
            getchar(); // Clear the newline
            fgets(searchText, sizeof(searchText), stdin);
            searchText[strcspn(searchText, "\n")] = '\0'; // Remove newline
            break;
        case 2:
            printf("Enter minimum price: ");
            scanf("%f", &minPrice);
            printf("Enter maximum price: ");
            scanf("%f", &maxPrice);
            break;
        case 3:
            printf("Enter text to search in product names: ");
            getchar(); // Clear the newline
            fgets(searchText, sizeof(searchText), stdin);
            searchText[strcspn(searchText, "\n")] = '\0'; // Remove newline

            printf("Enter minimum price: ");
            scanf("%f", &minPrice);
            printf("Enter maximum price: ");
            scanf("%f", &maxPrice);
            break;
        default:
            printf("Invalid choice. Showing all products.\n");
    }

    printf("\nFiltered Products:\n");
    printf("ID | Name            | Price   | Stock\n");
    printf("--------------------------------------\n");

    for (int i = 0; i < productCount; i++) {
        int matchesName = 1; // Default to match
        int matchesPrice = 1; // Default to match

        // Check name filter if applicable
        if (choice == 1 || choice == 3) {
            matchesName = 0; // Reset to no match
            if (searchText[0] != '\0') {
                char lowerName[100];
                char lowerSearchText[100];

                // Convert product name to lowercase
                for (int j = 0; products[i].name[j]; j++) {
                    lowerName[j] = tolower(products[i].name[j]);
                    lowerName[j + 1] = '\0';
                }

                // Convert search text to lowercase
                for (int j = 0; searchText[j]; j++) {
                    lowerSearchText[j] = tolower(searchText[j]);
                    lowerSearchText[j + 1] = '\0';
                }

                if (strstr(lowerName, lowerSearchText) != NULL) {
                    matchesName = 1;
                }
            }
        }

        // Check price filter if applicable
        if (choice == 2 || choice == 3) {
            if (!(minPrice <= products[i].price && products[i].price <= maxPrice)) {
                matchesPrice = 0;
            }
        }

        // If the product matches the selected filters, print it
        if (matchesName && matchesPrice) {
            printf("%d | %-15s | %.2f | %d\n",
                   products[i].id, products[i].name, products[i].price, products[i].stock);
        }
    }
}

// Get product by ID
Product getProductById(int productId) {
    for (int i = 0; i < productCount; i++) {
        if (products[i].id == productId) {
            return products[i];  // Return the product if found
        }
    }

    // Return a product with id -1 if not found
    Product notFoundProduct = {-1, "", -1.0f, 0};  // Default values indicating "not found"
    return notFoundProduct;
}
