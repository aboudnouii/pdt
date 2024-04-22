#include <stdio.h>
typedef struct PDT
{   //PDT details
    int state; // 1-free 0-not free
    float size;
}PDT;
typedef struct P
{   //process details
    int state; // 1-free 0-not free
    int id;
    float size;
    int pn;
} P;
typedef struct WQ
{
    int i;
    int id;//waiting zone
} WQ;
void read(PDT a[] , int n)  //reading PDT
{
    for(int i = 0 ; i < n ; i++)
    {
        a[i].state=0;
        printf(" the size of partition %d:",i+1);
        scanf("%f",&a[i].size);
    }
}
void readprocess(struct P a[] , int n)
{
     for(int i=0; i<n; i++)
    {
        printf(" the id of process %d:",i+1);
        scanf("%d",&a[i].id);
        printf(" the size of process %d:",i+1);
        scanf("%f",&a[i].size);
        a[i].state=0;
    }
}
void Display_PDT(PDT P[], int n) {
    int i;
    printf("Partition\tState\tSize\n");
    for(i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t%d\n", i + 1, P[i].state, P[i].size);
    }
}
void Display_P(P Q[], int m) {
    int i;
    printf("Processes\tSize\n");
    for(i = 0; i < m; i++) {
        if(Q[i].state == 1) {
            printf("%d\t\t%.d\n", i+1, Q[i].size);
        }
    }
}
void first_f(PDT P[],struct P Q[],WQ queue[],float ii[] ,int z,int n)
{
    int b;
    for(int i=0;i<n;i++)
    {
    ii[i] = 0;
    }
    b = 0;
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < z; j++) {
                if(P[j].state == 0 && Q[i].state == 0 && P[j].size >= Q[i].size) {
                    ii[j] = P[j].size - Q[i].size;
                    P[j].state = 1;
                    Q[i].state = 1;
                    Q[i].pn = j+1;
                    printf("Process id : %d of size %.2f allocated to partition %d.\n",Q[i].id, Q[i].size, j + 1);
                    break;
                }
                else
                {
                    queue[b].i=b;
                    queue[b].id=Q[i].id;
                    b=b++;
                    printf("Process id : %d of size %.2f is on queue zone %d.\n",Q[i].id, queue[b].i, j + 1);
                }
            }
        }
                for(int i=0;i<n;i++){
            if(P[i].state == 0){
                ii[i] = P[i].size;
            }
        }
        float iii=0;
                for(int i=0;i<n;i++){
        printf("InternalFragmentation for partition %d : %f\n",i+1,ii[i]);
        iii += ii[i]; 
    }
        printf("ExternalFragmentation : %f\n",iii);
}
void best_f(PDT P[], QWP Q[], float ii[], int n, int m) 
{
    int i, j, c, k, b, choice;
    best t[n];
    best x;
    float iii;
    char s;
    for(i=0;i<n;i++){
    ii[i] = 0;
    }
    iii = 0;
    b = 0;
    for(i=0;i<m;i++){
    c=0;
    for(j=0;j<n;j++){
    if(P[j].state == 0 && Q[i].state == 0 && P[j].size>=Q[i].size){
      t[c].size = P[j].size;
      t[c].n = j+1;
      c++;
    }
    }
    for(j=0;j<c;j++){
    for(k=j;k<c;k++){
      if(t[k].size < t[i].size){
      x = t[j];
      t[j] = t[k];
      t[k] = x;
      }
    }
    }
    P[t[0].pn - 1].state = 1;
    Q[i].state = 1;
    Q[i].n = t[0].n;
    f_f[t[0].pn - 1] = P[t[0].pn - 1].size - Q[i].size;
    }
    for(i=0;i<n;i++){
        printf("InternalFragmentation for partition %d : %f\n",i+1,ii[i]);
        iii += ii[i]; 
    }
    printf("ExternalFragmentation : %f\n",iii);
}

int main()
{
    int n,z;
    printf("number of the partirion:");
    scanf("%d",&z);
    PDT t[z];
    read(t,z);
    printf("number of processes:");
    scanf("%d",&n);
    P process[n];
    WQ queue[n];
    readprocess(process,n);
    Display_PDT(t,z);
    Display_P(process,n);
    float ii[z];
    for(int i = 0; i < n; i++)
    {
        ii[i] = 0;
    }
    first_f(t,process,queue,ii,z,n);

}
