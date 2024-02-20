#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    int N,M,i,b,j,n,k,c;
    printf("Oyun alaninin boyutlarini girin: ");        /*Kullanıcıdan oyun alanın büyüklüğünün ve yemek sayısının alındığı ksıım */
    scanf(" %d",&N);
    scanf(" %d",&M);
    printf("\nYemek sayisini girin:");
    scanf(" %d",&b);
    while (b>N*M)
    {
        printf("Lutfen uygun sayida yemek sayisi girin: \n"); /*eğer kullanıcı oyun alanına sığabileceğinde daha büyük bi değer girerse ekrana uyarı verip yeniden değer alan kısım.*/
        scanf(" %d",&b);
    }
    
    n=b;
    char A[N+2][M+2];
    for ( i = 0; i < N+2; i++)              /*Bu satırdan 99. satıra olan kısım oyun alanını oluşturup yazdırıyor.*/
    {
        if (i==0 || i==N+1)
        {
            for ( j = 0; j < M+2; j++)
           {
                A[i][j]='#';
           }
        }
        else
        {
            for ( j = 0; j < M+2; j=j+M+1)
            {
                A[i][j]='#';
            }
            
            
        }
        
        
    }

    int z,x;    
    srand(time(NULL));    
    for ( i = 1; i < b; i++)
    {
        z=(rand() % (N - 1))+1;
        x=(rand() % (M - 1))+1;
       if (A[z][x]!='0')
       {
        A[z][x]='0';
       }
       else
       {
        b=b+1;
       }       
       
    }

    int q,w;
    q=(rand() % (N - 1))+1;
    w=(rand() % (M - 1))+1;
    while (A[q][w]=='0')
    {
        q=(rand() % (N - 1))+1;
        w=(rand() % (M - 1))+1;
    }       
    A[q][w]='1';
    for ( i = 1; i < N+1; i++)
    {
        for ( j = 1; j < M+1; j++)
        {
            if (A[i][j]!='1' && A[i][j]!='0')
            {
                A[i][j]=' ';
            }            
        }        
    }

    int ko1,ko2;


    int Y[50][50];
    
        
    for ( i = 0; i < N+2; i++)
    {
        for ( j = 0; j < M+2; j++)
        {
           printf(" %c",A[i][j]);   
        }
        printf("\n");
    }
    printf("Kullanabileceginiz komutlar:\n");
    printf("Yukari icin: U\n");
    printf("Saga icin: R\n");
    printf("Asagi icin: D\n");
    printf("Sola icin: L\n");

    c=n;
    int uz=1,hm=0;
    int B[50][50];
    char tmp,mo1;
    while (n!=0)
    {
        printf("Hamleniz: \n");
        scanf("%c",&mo1);

        for ( i = 1; i < N+1; i++)
        {
            for ( j = 1; j < M+1; j++)
            {
                if (A[i][j]=='1')
                {
                    ko1=i;
                    ko2=j;
                }
            
            }
        
        }

        Y[0][0]=ko1;
        Y[0][1]=ko2;


        if (mo1=='U')
        {

            if (A[ko1-1][ko2]=='#')         /*Oyuncunun girdiği hamlenin oyun alanının dışına çıkıp çıkmadığını kontrol eden kısım eğer çıkıyorsa n'i sıfıra eşitleyerek oyunu bitiriyor*/
            {
                for ( i = 1; i < N+1; i++)
                {
                    for ( j = 1; j < M+1; j++)
                    {
                        if (A[i][j]!='0')
                        {
                            A[i][j]=' ';
                        }
                    
                    }
                
                }
            printf("Oyun alaninin dişina ciktiniz!!! \n");
            n=0;
            }
            else
            {
                if (A[ko1-1][ko2]!='0' && A[ko1-1][ko2]!=' ')       /*Bu kısım eğer yılan kendi kuyruğuna çarparsa oyunu bitirmek için.*/
                {
                    for ( i = 1; i < N+1; i++)
                    {
                        for ( j = 1; j < M+1; j++)
                        {
                            if (A[i][j]!='0')
                            {
                            A[i][j]=' ';
                            }
                    
                        }
                                    
                    }

                    printf("Yilan kendi kuyrugunu isirdi!!!\n");
                    n=0;
                }
                else
                {
                    if (A[ko1-1][ko2]=='0')         /*Bu ksıım eğer kullanıcının hamlesinden sonra yılanın gittiği yer '0' yani yemekse yılanı uzatmak ve hareket ettirmek için.*/
                    {
                        A[ko1-1][ko2]=' ';

                        tmp=A[ko1-1][ko2];
                        A[ko1-1][ko2]=A[ko1][ko2];
                        A[ko1][ko2]=tmp;

                        for ( i = 0; i < 2; i++)
                        {
                            Y[uz][i]=Y[uz-1][i];
                        }
                        for ( i = 0; i < uz; i++)
                        {
                            for ( j = 0; j < 2; j++)
                            {
                                B[i][j]=Y[i][j];
                            }
                        
                        }

                        for ( i = uz-1; i >0; i--)
                        {
                            for ( j = 0; j < 2; j++)
                            {
                                Y[i][j]=Y[i-1][j];
                            }
                        
                        }

                        for ( i = 1; i < uz; i++)
                        {
                            tmp=A[Y[i][0]][Y[i][1]];
                            A[Y[i][0]][Y[i][1]]=A[B[i][0]][B[i][1]];
                            A[B[i][0]][B[i][1]]=tmp;
                        }

                        Y[0][0]=ko1-1;
                        A[Y[uz][0]][Y[uz][1]]=A[Y[uz-1][0]][Y[uz-1][1]]+1;
                        uz=uz+1;
                        n=n-1;
                        c=c-1;

                    }
                    else            /*Bu kısım eğer yılan boşluğa gidiyorsa onu hareket ettirmek için.*/
                    {
                        tmp=A[ko1-1][ko2];
                        A[ko1-1][ko2]=A[ko1][ko2];
                        A[ko1][ko2]=tmp;

                        for ( i = 0; i < uz; i++)
                        {
                            for ( j = 0; j < 2; j++)
                            {
                                B[i][j]=Y[i][j];
                            }
                        
                        }

                        for ( i = uz-1; i >0; i--)
                        {
                            for ( j = 0; j < 2; j++)
                            {
                                Y[i][j]=Y[i-1][j];
                            }
                        
                        }

                        for ( i = 1; i < uz; i++)
                        {
                            tmp=A[Y[i][0]][Y[i][1]];
                            A[Y[i][0]][Y[i][1]]=A[B[i][0]][B[i][1]];
                            A[B[i][0]][B[i][1]]=tmp;
                        }

                    

                    }
                }
                                
            }
            
            
        }
        if (mo1=='R')
        {
            if (A[ko1][ko2+1]=='#')
            {
                for ( i = 1; i < N+1; i++)
                {
                    for ( j = 1; j < M+1; j++)
                    {
                        if (A[i][j]!='0')
                        {
                        A[i][j]=' ';
                        }
                    
                    }
                
                }
            printf("Oyun alaninin dişina ciktiniz!!! \n");
            n=0;
            }
            else
            {
                if (A[ko1][ko2+1]!='0' && A[ko1][ko2+1]!=' ')
                {
                    for ( i = 1; i < N+1; i++)
                    {
                        for ( j = 1; j < M+1; j++)
                        {
                            if (A[i][j]!='0')
                            {
                            A[i][j]=' ';
                            }
                    
                        }
                
                    }
                    printf("Yilan kendini isirdi!!!\n");
                    n=0;
                }
                else
                {
                    if (A[ko1][ko2+1]=='0')
                    {
                        A[ko1][ko2+1]=' ';

                        tmp=A[ko1][ko2+1];
                        A[ko1][ko2+1]=A[ko1][ko2];
                        A[ko1][ko2]=tmp;

                        for ( i = 0; i < 2; i++)
                        {
                            Y[uz][i]=Y[uz-1][i];
                        }
                        for ( i = 0; i < uz; i++)
                        {
                            for ( j = 0; j < 2; j++)
                            {
                                B[i][j]=Y[i][j];
                            }
                        
                        }

                        for ( i = uz-1; i >0; i--)
                        {
                            for ( j = 0; j < 2; j++)
                            {
                                Y[i][j]=Y[i-1][j];
                            }
                        
                        }

                        for ( i = 1; i < uz; i++)
                        {
                            tmp=A[Y[i][0]][Y[i][1]];
                            A[Y[i][0]][Y[i][1]]=A[B[i][0]][B[i][1]];
                            A[B[i][0]][B[i][1]]=tmp;
                        }

                        Y[0][1]=ko2+1;
                        A[Y[uz][0]][Y[uz][1]]=A[Y[uz-1][0]][Y[uz-1][1]]+1;
                        uz=uz+1;
                        n=n-1;
                         c=c-1;

                    }
                    else
                    {
                        tmp=A[ko1][ko2+1];
                        A[ko1][ko2+1]=A[ko1][ko2];
                        A[ko1][ko2]=tmp;

                        for ( i = 0; i < uz; i++)
                        {
                            for ( j = 0; j < 2; j++)
                            {
                                B[i][j]=Y[i][j];
                            }
                        
                        }

                        for ( i = uz-1; i >0; i--)
                        {
                            for ( j = 0; j < 2; j++)
                            {
                                Y[i][j]=Y[i-1][j];
                            }
                        
                        }

                        for ( i = 1; i < uz; i++)
                        {
                            tmp=A[Y[i][0]][Y[i][1]];
                            A[Y[i][0]][Y[i][1]]=A[B[i][0]][B[i][1]];
                            A[B[i][0]][B[i][1]]=tmp;
                        }

                    

                    }
                }
                                
            }
            
            
        }


        if (mo1=='D')
        {

            if (A[ko1+1][ko2]=='#')
            {
                for ( i = 1; i < N+1; i++)
                {
                    for ( j = 1; j < M+1; j++)
                    {
                        if (A[i][j]!='0')
                        {
                        A[i][j]=' ';
                        }
                    
                    }
                
                }
            printf("Oyun alaninin dişina ciktiniz!!! \n");
            n=0;
            }
            else
            {
                if (A[ko1+1][ko2]!='0' && A[ko1+1][ko2]!=' ')
                {
                    for ( i = 1; i < N+1; i++)
                    {
                        for ( j = 1; j < M+1; j++)
                        {
                            if (A[i][j]!='0')
                            {
                            A[i][j]=' ';
                            }
                    
                        }
                
                    }

                    printf("Yilan kendini isirdi!!!\n");
                    n=0;
                }
                else
                {
                    if (A[ko1+1][ko2]=='0')
                    {
                        A[ko1+1][ko2]=' ';

                        tmp=A[ko1+1][ko2];
                        A[ko1+1][ko2]=A[ko1][ko2];
                        A[ko1][ko2]=tmp;

                        for ( i = 0; i < 2; i++)
                        {
                            Y[uz][i]=Y[uz-1][i];
                        }
                        for ( i = 0; i < uz; i++)
                        {
                            for ( j = 0; j < 2; j++)
                            {
                                B[i][j]=Y[i][j];
                            }
                        
                        }

                        for ( i = uz-1; i >0; i--)
                        {
                            for ( j = 0; j < 2; j++)
                            {
                                Y[i][j]=Y[i-1][j];
                            }
                        
                        }

                        for ( i = 1; i < uz; i++)
                        {
                            tmp=A[Y[i][0]][Y[i][1]];
                            A[Y[i][0]][Y[i][1]]=A[B[i][0]][B[i][1]];
                            A[B[i][0]][B[i][1]]=tmp;
                        }

                        Y[0][0]=ko1+1;
                        A[Y[uz][0]][Y[uz][1]]=A[Y[uz-1][0]][Y[uz-1][1]]+1;
                        uz=uz+1;
                        n=n-1;
                        c=c-1;

                    }
                    else
                    {
                        tmp=A[ko1+1][ko2];
                        A[ko1+1][ko2]=A[ko1][ko2];
                        A[ko1][ko2]=tmp;

                        for ( i = 0; i < uz; i++)
                        {
                            for ( j = 0; j < 2; j++)
                            {
                                B[i][j]=Y[i][j];
                            }
                        
                        }

                        for ( i = uz-1; i >0; i--)
                        {
                            for ( j = 0; j < 2; j++)
                            {
                                Y[i][j]=Y[i-1][j];
                            }
                        
                        }

                        for ( i = 1; i < uz; i++)
                        {
                            tmp=A[Y[i][0]][Y[i][1]];
                            A[Y[i][0]][Y[i][1]]=A[B[i][0]][B[i][1]];
                            A[B[i][0]][B[i][1]]=tmp;
                        }
                    }
                }               
            }
        }
        if (mo1=='L')
        {
            if (A[ko1][ko2-1]=='#')
            {
                 for ( i = 1; i < N+1; i++)
                 {
                    for ( j = 1; j < M+1; j++)
                    {
                        if (A[i][j]!='0')
                        {
                        A[i][j]=' ';
                        }
                    }
                 }
            printf("Oyun alaninin dişina ciktiniz!!! \n");
            n=0;
            }
            else
            {
                if (A[ko1][ko2-1]!='0' && A[ko1][ko2-1]!=' ')
                {
                    for ( i = 1; i < N+1; i++)
                    {
                        for ( j = 1; j < M+1; j++)
                        {
                            if (A[i][j]!='0')
                            {
                                A[i][j]=' ';
                            }
                    
                        }
                
                    }
                    printf("Yilan kendini isirdi!!! \n");
                    n=0;
                    
                }
                else
                {
                    if (A[ko1][ko2-1]=='0')
                    {
                        A[ko1][ko2-1]=' ';

                        tmp=A[ko1][ko2-1];
                        A[ko1][ko2-1]=A[ko1][ko2];
                        A[ko1][ko2]=tmp;

                        for ( i = 0; i < 2; i++)
                        {
                            Y[uz][i]=Y[uz-1][i];
                        }
                        for ( i = 0; i < uz; i++)
                        {
                            for ( j = 0; j < 2; j++)
                            {
                                B[i][j]=Y[i][j];
                            }
                        
                        }

                        for ( i = uz-1; i >0; i--)
                        {
                            for ( j = 0; j < 2; j++)
                            {
                             Y[i][j]=Y[i-1][j];
                            }
                        
                        }

                        for ( i = 1; i < uz; i++)
                        {
                            tmp=A[Y[i][0]][Y[i][1]];
                            A[Y[i][0]][Y[i][1]]=A[B[i][0]][B[i][1]];
                            A[B[i][0]][B[i][1]]=tmp;
                        }

                        Y[0][1]=ko2-1;
                        A[Y[uz][0]][Y[uz][1]]=A[Y[uz-1][0]][Y[uz-1][1]]+1;
                        uz=uz+1;
                        n=n-1;
                        c=c-1;

                    }
                    else
                    {
                        tmp=A[ko1][ko2-1];
                        A[ko1][ko2-1]=A[ko1][ko2];
                        A[ko1][ko2]=tmp;

                        for ( i = 0; i < uz; i++)
                        {
                            for ( j = 0; j < 2; j++)
                            {
                                B[i][j]=Y[i][j];
                            }
                        
                        }

                        for ( i = uz-1; i >0; i--)
                        {
                            for ( j = 0; j < 2; j++)
                            {
                                Y[i][j]=Y[i-1][j];
                            }
                        
                        }

                        for ( i = 1; i < uz; i++)
                        {
                            tmp=A[Y[i][0]][Y[i][1]];
                            A[Y[i][0]][Y[i][1]]=A[B[i][0]][B[i][1]];
                            A[B[i][0]][B[i][1]]=tmp;
                        }

                    

                    }
                }
                          
            }
            
            
        }


        for ( i = 0; i < N+2; i++)              /*Burası oyuncunun hamlesi sonucunda oluşan oyun alanını göstermek için.*/
        {
            for ( j = 0; j < M+2; j++)
            {
                printf(" %c",A[i][j]);
            }

            printf("\n");
        }
        
        hm=hm+1;
    }
    printf("OYUN BITTI!!! \n");
    printf("Yaptiginiz hamle sayisi: %d \n",hm);
    printf("Kalan yemek sayisi: %d \n",c);

    return 0;
}