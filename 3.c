#include<stdio.h>
struct rtable
{
int dist[20],nextnode[20];
}table[20];
int cost[10][10],n;
void distvector()
{
int i,j,k,count=0;
for(i=0;i<n;i++)
{
for(j=0;j<n;j++)
{
table[i].dist[j]=cost[i][j];
table[i].nextnode[j]=j;
}
}
 do
 {
 count=0;
for(i=0;i<n;i++)
{
for(j=0;j<n;j++)
{
for(k=0;k<n;k++)
{
if(table[i].dist[j]>cost[i][k]+table[k].dist[j])
{
table[i].dist[j]=table[i].dist[k]+table[k].dist[j];
table[i].nextnode[j]=k;
count++;
}
}
}
}
}while(count!=0);
}
int main()
{
int i,j;
printf("\nenter the no of vertices:\t");
scanf("%d",&n);
printf("\nenter the cost matrix\n");
for(i=0;i<n;i++)
for(j=0;j<n;j++)
scanf("%d",&cost[i][j]);
distvector();
for(i=0;i<n;i++)
{
printf("\nstate value for router %c \n",i+65);
printf("\ndestnode\tnextnode\tdistance\n");
for(j=0;j<n;j++)
{
if(table[i].dist[j]==99)
printf("%c\t\t-\t\tinfinite\n",j+65);
else
printf("%c\t\t%c\t\t%d\n",j+65,table[i].nextnode[j]+65,table[i].dist[j]);
} }
return 0; 
}
OUTPUT
$ gcc 4.c
$ ./a.out
enter the no of vertices: 6
enter the cost matrix
0 2 1 99 99 1
2 0 99 2 1 99
1 99 0 99 2 2
99 2 99 0 1 5
99 1 2 1 0 99
1 99 2 5 99 0
state value for router A 
destnode nextnode distance
A A 0
B B 2
C C 1
D B 4
E B 3
F F 1
state value for router B 
destnode nextnode distance
A A 2
B B 0
C A 3
D D 2
E E 1
F A 3
state value for router C 
destnode nextnode distance
A A 1
B A 3
C C 0
D E 3
E E 2
F F 2
state value for router D 
destnode nextnode distance
A B 4
B B 2
C E 3
D D 0
E E 1
F F 5
state value for router E 
destnode nextnode distance
A B 3
B B 1
C C 2
D D 1
E E 0
F B 4
state value for router F 
destnode nextnode distance
A A 1
B A 3
C C 2
D D 5
E A 4
F F 0
