#ifndef ORDER_H
#define ORDER_H

#include "cart.h"
#include "voucher.h"

typedef enum {
    HOME_DELIVERY,
    LOCAL_PICKUP
} DeliveryType;

typedef struct {
    Cart cart;
    DeliveryType deliveryType;
    char promoCode[21]; // 20 characters + null terminator
    float finalPrice;
    char userId[50]; // User ID for tracking orders
    struct tm orderDate; // New field for order date
} Order;

// Function declarations
Order createOrder(Cart *cart, DeliveryType deliveryType, const char *promoCode, const char *userId);
float calculateFinalPrice(Cart cart, DeliveryType deliveryType, const Voucher *voucher);
void displayOrder(Order order);
void saveOrderToFile(Order order);
void listAllOrders();
void listOrdersByUserId(const char *userId);
void listOrdersByDateRange(struct tm startDate, struct tm endDate);
void inputDate(struct tm *date, const char *prompt);
void handleListOrdersByDateRange();

#endif
