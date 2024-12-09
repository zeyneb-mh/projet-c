#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "voucher.h"

#define VOUCHER_FILE "vouchers.txt"

Voucher vouchers[100];
int voucherCount = 0;


void generateCode(char *code) {
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    int isUnique;
    do {
        isUnique = 1;
        for (int i = 0; i < 20; i++) {
            code[i] = charset[rand() % (sizeof(charset) - 1)];
        }
        code[20] = '\0';


        for (int i = 0; i < voucherCount; i++) {
            if (strcmp(vouchers[i].code, code) == 0) {
                isUnique = 0;
                break;
            }
        }
    } while (!isUnique);
}


void loadVouchers() {
    FILE *file = fopen(VOUCHER_FILE, "r");
    if (file == NULL) {
        voucherCount=0;
        return;
    }
    voucherCount=0;
    while (fscanf(file, "%d\n%f\n%99[^\n]\n%20s\n%d\n",
                  &vouchers[voucherCount].id,
                  &vouchers[voucherCount].discount,
                  vouchers[voucherCount].description,
                  vouchers[voucherCount].code,
                  (int *)&vouchers[voucherCount].state) != EOF) {
        voucherCount++;
    }

    fclose(file);
}


void saveVoucherToFile(Voucher voucher) {
    FILE *file = fopen(VOUCHER_FILE, "a");
    if (file == NULL) {
        printf("Error: Unable to open vouchers file for writing.\n");
        return;
    }

    fprintf(file, "%d\n%.2f\n%s\n%s\n%d\n",
            voucher.id,
            voucher.discount,
            voucher.description,
            voucher.code,
            voucher.state);
    fclose(file);
}


void addVoucher(Voucher voucher) {
    voucher.id = voucherCount + 1;
    generateCode(voucher.code);
    voucher.state = UNUSED;

    vouchers[voucherCount] = voucher;
    voucherCount++;

    saveVoucherToFile(voucher);
    printf("Voucher added successfully! Code: %s\n", voucher.code);
}


void modifyVoucher(int id, Voucher newVoucher) {
    for (int i = 0; i < voucherCount; i++) {
        if (vouchers[i].id == id) {
            vouchers[i] = newVoucher;
            vouchers[i].id = id;


            FILE *file = fopen(VOUCHER_FILE, "w");
            if (file == NULL) {
                printf("Error: Unable to open vouchers file for rewriting.\n");
                return;
            }

            for (int j = 0; j < voucherCount; j++) {
                fprintf(file, "%d\n%.2f\n%s\n%s\n%d\n",
                        vouchers[j].id,
                        vouchers[j].discount,
                        vouchers[j].description,
                        vouchers[j].code,
                        vouchers[j].state);
            }
            fclose(file);

            printf("Voucher modified successfully!\n");
            return;
        }
    }

    printf("Voucher with ID %d not found.\n", id);
}


void deleteVoucher(int id) {
    for (int i = 0; i < voucherCount; i++) {
        if (vouchers[i].id == id) {
            for (int j = i; j < voucherCount - 1; j++) {
                vouchers[j] = vouchers[j + 1];
            }

            voucherCount--;

            FILE *file = fopen(VOUCHER_FILE, "w");
            if (file == NULL) {
                printf("Error: Unable to open vouchers file for rewriting.\n");
                return;
            }

            for (int j = 0; j < voucherCount; j++) {
                fprintf(file, "%d\n%.2f\n%s\n%s\n%d\n",
                        vouchers[j].id,
                        vouchers[j].discount,
                        vouchers[j].description,
                        vouchers[j].code,
                        vouchers[j].state);
            }
            fclose(file);

            printf("Voucher deleted successfully!\n");
            return;
        }
    }

    printf("Voucher with ID %d not found.\n", id);
}


void listVouchers() {
    if (voucherCount == 0) {
        printf("No vouchers available.\n");
        return;
    }

    printf("ID | Discount | Description | Code | State\n");
    printf("---------------------------------------------\n");
    for (int i = 0; i < voucherCount; i++) {
        printf("%d | %.2f%% | %s | %s | %s\n",
               vouchers[i].id,
               vouchers[i].discount,
               vouchers[i].description,
               vouchers[i].code,
               vouchers[i].state == UNUSED ? "UNUSED" : "USED");
    }
}


Voucher *findVoucherByCode(const char *code) {
    loadVouchers();
    for (int i = 0; i < voucherCount; i++) {
        if (strcmp(vouchers[i].code, code) == 0) {
            return &vouchers[i];
        }
    }
    return NULL;
}


int validateVoucher(Voucher *voucher) {
    if (voucher->state == USED) {
        printf("Voucher '%s' has already been used and cannot be applied again.\n", voucher->code);
        return 0;
    }
    printf("Voucher '%s' is valid and provides a %.2f%% discount.\n", voucher->code, voucher->discount);
    return 1;

}
void markVoucherUsed(Voucher *voucher) {
    if (voucher->state == UNUSED) {
        voucher->state = USED;
        printf("Voucher '%s' has been marked as used.\n", voucher->code);
    }
}
