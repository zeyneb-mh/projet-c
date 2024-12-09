#ifndef VOUCHER_H
#define VOUCHER_H

typedef enum {
    UNUSED,
    USED
} VoucherState;

typedef struct {
    int id;
    float discount;
    char description[100];
    char code[21];
    VoucherState state;
} Voucher;

extern Voucher vouchers[100];
extern int voucherCount;


void loadVouchers();
void saveVoucherToFile(Voucher voucher);
void addVoucher(Voucher voucher);
void modifyVoucher(int id, Voucher newVoucher);
void deleteVoucher(int id);
void listVouchers();
Voucher *findVoucherByCode(const char *code);
int validateVoucher(Voucher *voucher);
void markVoucherUsed(Voucher *voucher);

#endif
