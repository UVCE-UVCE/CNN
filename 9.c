#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))
#define MAX 25

int main() {
    int cap, oprt, cont, i = 0, inp[MAX], ch, nsec, drop;

    printf("LEAKY BUCKET ALGORITHM\n");
    printf("\nEnter the bucket size:\n");
    scanf("%d", &cap);
    printf("\nEnter the output rate:");
    scanf("%d", &oprt);

    do {
        printf("\nEnter the number of packets entering at %d seconds\n", i + 1);
        scanf("%d", &inp[i]);
        i++;
        printf("\nEnter 1 to insert packet or 0 to quit\n");
        scanf("%d", &ch);
    } while (ch);

    nsec = i;

    printf("\n(SECOND):(PACK RECVD):(PACK SENT):(PACK LEFT IN BUCKET):(PACK DROPPED)\n");

    cont = 0;
    drop = 0;
    for (i = 0; i < nsec; i++) {
        cont += inp[i];
        if (cont > cap) {
            drop = cont - cap;
            cont = cap;
        }
        printf("(%d): ", i + 1);
        printf("\t\t(%d): ", inp[i]);
        printf("\t\t(%d): ", min(cont, oprt));
        cont = cont - min(cont, oprt);
        printf("\t\t(%d)", cont);
        printf("\t\t(%d)\n", drop);
    }

    for (; cont != 0; i++) {
        if (cont > cap)
            cont = cap;
        drop = 0;
        printf("(%d): ", i + 1);
        printf("\t\t(0): ");
        printf("\t\t(%d): ", min(cont, oprt));
        cont = cont - min(cont, oprt);
        printf("\t\t(%d)", cont);
        printf("\t\t(%d)\n", drop);
    }

    return 0;
}
OUTPUT
$ gcc 11.c
$ ./a.out
LEAKY BUCKET ALGORITM
Enter the bucket size:
10
Enter the output rate:4
Enter the number of packets entering at 1 seconds
6
Enter 1 to insert packet or 0 to quit
1
Enter the number of packets entering at 2 seconds
8
Enter 1 to insert packet or 0 to quit
1
Enter the number of packets entering at 3 seconds
12
Enter 1 to insert packet or 0 to quit
1
Enter the number of packets entering at 4 seconds
20
Enter 1 to insert packet or 0 to quit
1
Enter the number of packets entering at 5 seconds
4
Enter 1 to insert packet or 0 to quit
0
(SECOND):(PACK RECVD):(PACK SENT):(PACK LEFT IN BUCKET):(PACK dROPPED)
(1): (6): (4): (2) (0)
(2): (8): (4): (6) (0)
(3): (12): (4): (6) (8)
(4): (20): (4): (6) (16)
(5): (4): (4): (6) (16)
(6): (0): (4): (2) (0)
(7): (0): (2): (0) (0)
