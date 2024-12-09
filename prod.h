#ifndef PRODUCT_H
#define PRODUCT_H
#define MAX_PRODUCTS 100

typedef struct {
    int id;
    char name[100];
    float price;
    int stock;
} Product;

extern Product products[MAX_PRODUCTS];
extern int productCount;


void loadProducts();
void addProduct(Product product);
void modifyProduct( int id, Product newProduct);
void deleteProduct( int id );
void listProducts();
void saveProductToFile(Product product);
void saveAllProducts();
void filterProducts();
Product getProductById(int productId);

#endif
