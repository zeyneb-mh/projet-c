#ifndef CART_H_INCLUDED
#define CART_H_INCLUDED
#include "prod.h"
#include "user.h"
#include "voucher.h"

typedef struct {
    Product product;
    int quantity;
    float totalCost;
} CartItem;

typedef struct {
    char userId[100];
    CartItem items[100];
    int itemCount;
} Cart;


Cart initializeCart(const char *userId);
void addProductToCart(Cart *cart, Product product, int quantity);
float calculateCartTotal(Cart cart);
void displayCart(Cart cart);
void saveCartToFile(Cart cart);
int loadCartFromFile(const char *userId, Cart *cart);
float applyVoucher(float total, Voucher voucher);
void removeProductFromCart(Cart *cart, int productId);
void editProductQuantity(Cart *cart, int productId, int newQuantity);


#endif
