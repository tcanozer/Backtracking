#include<stdio.h>
#include<stdlib.h>
#include <string.h>


//secilen satirin elemanlarini dairesel olarak 1 birim saga kaydiran fonksiyon
void rightShift(int n, int **colorMatrix, int currentRow){
    
    int i;
    int last;
    last = colorMatrix[currentRow][n-1];
    
    for(i=n-1;i>=0;i--){
        colorMatrix[currentRow][i] = colorMatrix[currentRow][i-1];
    }
    colorMatrix[currentRow][0] = last;
}

//secilen satirin elemanlarinin ustundeki satir elemanlarini
//kontrol ederek ilgili elemanin bir ust satirda olup olmadigini bilgisini
//donduren fonksiyon
int checkColumn(int n, int **colorMatrix, int currentRow){
    int i,j;
    
    for(i=0;i<currentRow;i++){
        for(j=0;j<n;j++){
            if(colorMatrix[currentRow][j]==colorMatrix[i][j])
                return 1;
        }
    }
    return 0;
}


void printResultMatrixColors(int n, int **colorMatrix, char **selectedColors){
    printf("\n\n");
    
    int i,j;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            printf("| %-7s ",selectedColors[colorMatrix[i][j]] );
        }
        printf(" |\n");
        printf("---------------------------------------------\n");
    }
    printf("\n");
    
}

//backtracking ile renk siralama isleminin yapildigi rekursif fonksiyon
void backtracking(int n, int **colorMatrix, int currentRow, int *rowShiftCount,int backFlag,int mode,char **selectedColors){
    
    if((currentRow >= 0) && (currentRow < n)){
        if(backFlag == 1){                              //ustteki satira geri donulduyse
            rightShift(n, colorMatrix, currentRow);
            rowShiftCount[currentRow]++;
        }
        if((checkColumn(n, colorMatrix, currentRow) == 1) && currentRow!=0){    //ilgili satir sarti saglamadiysa saga kaydir
            if(rowShiftCount[currentRow] < n){
                rightShift(n, colorMatrix, currentRow);
                rowShiftCount[currentRow]++;
                backtracking(n, colorMatrix, currentRow, rowShiftCount, 0,mode,selectedColors);
            }
        }
        if(rowShiftCount[currentRow] == n && currentRow!=0){                //eger ilgili satiri n kez kaydirdiysan ustteki satira don
            if(mode == 2){
                printf("Sart saglanamadi %d. satira geri donuluyor\n",currentRow);
            }
            rowShiftCount[currentRow] = 0;
            backtracking(n, colorMatrix, currentRow-1, rowShiftCount,1,mode,selectedColors);
        }
        else if(checkColumn(n, colorMatrix, currentRow) == 0){  //ilgili satir sarti sagladi
            if(currentRow == 0){
                printf("\nBu giriş bilgisi için sonuç yoktur \n");
                exit(0);
            }else{
                if(currentRow+1 == n){
                    if(mode == 2){
                        printf("%d . satir ust satirlara gore duzenlenmistir : ",currentRow+1);
                        printResultMatrixColors(n,colorMatrix,selectedColors);
                    }
                    printf("\nSonuc Bulunmustur: \n");
                    printResultMatrixColors(n,colorMatrix,selectedColors);
                    exit(0);
                }
            }
            if(mode == 2){
                printf("\n%d . satir ust satirlara gore duzenlenmistir:",currentRow+1);
                printResultMatrixColors(n,colorMatrix,selectedColors);
            }
            backtracking(n, colorMatrix, currentRow+1, rowShiftCount, 0,mode,selectedColors); // ilgili satir sarti sagladiysa bir sonraki satira gec
        }
    }else{
        return;
    }
}

void getColorMatrix(int n, int **colorMatrix, char *colorList[8], char **selectedColors){
    
    int i,j=0,k=0;
    char color[10];
    
    printf("\n\nRenk Listesi: \n");
    for(i=0;i<8;i++){
        printf("(%s) ",colorList[i]);
    }
    printf("\n-------------------------------------------------------------------------------------\n\n");
        
    for(i=0;i<n;i++){
        printf("Kullanmak istediginiz %d adet renkten %d. yi giriniz : ",n,i+1);
        scanf("%s",color);
        while(strcmp(color,colorList[k])!=0 && k<8){
            k++;
        }
        if(k>=8 ){
            printf("\nLutfen gecerli bir renk bilgisi giriniz! \n");
            i--;
            k=0;
        }else{
            while(strcmp(selectedColors[j],color)!=0 && j<n-1){
                j++;
            }if(j<n-1){
                printf("\nLutfen oncekilerden farkli bir renk bilgisi giriniz! \n");
                i--;
                k=0;
                j=0;
            }
            else{
                strcpy(selectedColors[i], color);
                k=0;
                j=0;
            }
        }
    }
    printf("\n");
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            printf("%d . satir %d. sutunun renk bilgisini giriniz: ",i+1,j+1);
            scanf("%s",color);
            while(strcmp(color,selectedColors[k])!=0 && k<n){
                k++;
            }
            if(k>=n ){
                printf("\nLutfen gecerli bir renk bilgisi giriniz! \n");
                j--;
                k=0;
            }else{
                colorMatrix[i][j] = k;
                k=0;
            }
        }
    }
    printf("\nRenk Matrisinin Sayisal Karsiligi: \n");
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            printf("%d ",colorMatrix[i][j]);
        }
        printf("\n");
    }
    
}

int main(){
    int i;
    int n=0;
    int mode;
    char *colorList[8] = {"siyah","kirmizi","beyaz","mavi","yesil","sari","mor","cyan"};
    int **colorMatrix;
    int *rowShiftCount;
    char **selectedColors;
    
    
    printf("3 <= N <= 8 olmak uzere N degerini giriniz: ");
    scanf("%d",&n);
    if((n < 3) || (n > 8)){
        printf("Gecersiz deger girildi.\nProgram sonlaniyor\n");
        return 0;
    }
    
    rowShiftCount = (int*)calloc(n,sizeof(int));
    selectedColors = (char**)malloc(n*sizeof(char*));
    colorMatrix = (int**)malloc(sizeof(int*)*n);
    for(i=0;i<n;i++){
        colorMatrix[i] = (int*)malloc(sizeof(int)*n);
        selectedColors[i] = (char*)malloc(10*sizeof(char));
    }
    
    printf("\nNormal Mode: 1, Detailed Mode: 2\n");
    printf("-------------------------------------------------------------------------------------\n\n");
    printf("Calistirmak istediginiz modu giriniz : ");
    scanf("%d",&mode);
    
    if(mode == 1){
        getColorMatrix(n,colorMatrix,colorList,selectedColors);
        backtracking(n, colorMatrix, 1, rowShiftCount, 0,mode,selectedColors);
    }else if(mode == 2){
        getColorMatrix(n,colorMatrix,colorList,selectedColors);
        backtracking(n, colorMatrix, 1, rowShiftCount, 0,mode,selectedColors);
    }else{
        printf("\n Gecersiz Deger Girildi \n\n");
        return 0;
    }
    
    for(i=0;i<n;i++){
        free(selectedColors[i]);
        free(colorMatrix[i]);
    }
    free(rowShiftCount);
    free(selectedColors);
    free(colorMatrix);
    
    return 0;
}
