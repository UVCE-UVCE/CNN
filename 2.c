#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char data[100], concatdata[117], src_crc[17], dest_crc[17], frame[120], divident[18], divisor[18] = "10001000000010001", res[17] = "0000000000000000";

void crc_cal(int node) {
    int i, j;
    for (j = 17; j < strlen(concatdata); j++) {
        if (divident[0] == '1') {
            for (i = 1; i <= 16; i++)
                divident[i - 1] = (divident[i] != divisor[i]) ? '1' : '0';
        } else {
            for (i = 1; i <= 16; i++)
                divident[i - 1] = divident[i];
        }
        divident[i - 1] = (node == 0) ? concatdata[j] : frame[j];
    }
    divident[i - 1] = '\0';
    printf("\nCRC is %s\n", divident);
    if (node == 0) {
        strcpy(src_crc, divident);
    } else {
        strcpy(dest_crc, divident);
    }
}

int main() {
    int i;
    printf("\n\t\t\tAT SOURCE NODE\n\nEnter the data to be sent: ");
    fgets(data, sizeof(data), stdin);
    data[strcspn(data, "\n")] = '\0'; // Remove newline character from fgets

    strcpy(concatdata, data);
    strcat(concatdata, "0000000000000000");

    // Initialize divident
    strncpy(divident, concatdata, 17);
    divident[17] = '\0';

    crc_cal(0);

    printf("\nData is: \t%s", data);
    printf("\nThe frame transmitted is: \t%s%s", data, src_crc);
    printf("\n\t\tSOURCE NODE TRANSMITTED THE FRAME --->");

    printf("\n\n\n\n\t\t\tAT DESTINATION NODE\nEnter the received frame:\t");
    fgets(frame, sizeof(frame), stdin);
    frame[strcspn(frame, "\n")] = '\0'; // Remove newline character from fgets

    // Initialize divident
    strncpy(divident, frame, 17);
    divident[17] = '\0';

    crc_cal(1);

    if (strcmp(dest_crc, res) == 0) {
        printf("\nReceived frame is error free");
    } else {
        printf("\nReceived frame has one or more errors");
    }

    return 0;

}
OUTPUT
$ gcc 3.c
$ ./a.out
AT SOURCE NODE
enter the data to be send :1101
crc is 1101000110101101
data is : 1101
the frame transmitted is :
11011101000110101101
SOURCE NODE TRANSMITTED THE FRAME ---->
AT DESTINATION NODE
enter the received frame: 11011101000110101101
crc is 0000000000000000
received frame is error free
~$ ./a.out
AT SOURCE NODE
enter the data to be send :110011 
crc is 0000011000110000
data is : 110011
the frame transmitted is :
1100110000011000110000
SOURCE NODE TRANSMITTED THE FRAME ---->
AT DESTINATION NODE
enter the received frame: 1100110000011000110001
crc is 0000000000000001
received frame has one or more error
