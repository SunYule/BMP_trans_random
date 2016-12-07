#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <time.h>

void main()
{
    //takeBack();
    int a=0;
    int random=0;
    srand(time(0));
    random=rand()%10+1; //生成一个随机数，为了测试方便，此处使用1到10之间的随机数
    printf("Random number: %d\n",random);
    do{
        printf("1.hiding\n2.pick up\nChoose: ");
        scanf("%d",&a);
        if(a==1)
            hiding(random);
        if(a==2)
            pickUp(random);
    }while(1);
}

void hiding(int random)
{
    FILE *in, *out;
    int i,j,m,n,r;  //计数器
    char carrierFile[50], outputFile[50];
    char *pinfo, info[100], *temp;
    printf("Carrier file: ");
    scanf("%s",carrierFile);
    printf("Output file: ");
    scanf("%s",outputFile);
    fflush(stdin);
    printf("Secret information:");
    pinfo = info;
    gets(pinfo);
    for(m=0, temp=(char *)malloc(8*strlen(info)); m<strlen(info); pinfo++, m++){
        for(n=0; n<8; n++){
            temp[8*m+n] = 0x01 & *pinfo>>n;
            //把第m个位置的字符的每一位依次取出来（移位后和0x01做按位与运算）
        }
    }
    if((in=fopen(carrierFile,"rb"))==NULL){
        printf("Carrier file fopen error.\n");
        return;
    }   //打开载体文件
    if((out=fopen(outputFile,"wb"))==NULL){
        printf("Output file fopen error.\n");
        return;
    }   //打开输出文件
    for(i=0,j=0; !feof(in); i++){
        if(i<54){
            fputc(fgetc(in),out);   //前54个字符为bmp文件头，保留
        }
        else
        {
            if(j<8*strlen(info)){
                //读一个字符，与0xfe做按位与运算，将其末位（LSB）置零
                fputc((fgetc(in)&0xfe) + temp[j], out);
                for(r=0; r<random; r++){
                    fputc(fgetc(in)&0xfe, out);
                }   //用fgetc函数跳过random个字符
            }
            else
                fputc(fgetc(in)&0xfe, out);
            j++;
        }
    }
    fclose(in);
    fclose(out);
    printf("Hiding success.\n");
}

void pickUp(int random)
{
    FILE *in;
    unsigned int i,r;   //计数器
    char FName[90], ch;
    printf("File to pickup: ");
    scanf("%s",FName);
    if((in=fopen(FName,"rb"))==NULL){
        printf("File fopen error.\n");
        return;
    }   //打开要提取的文件
    fseek(in,54L,0);    //使用fseek跳过54字节的头
    printf("Secret information: ");
    do{
        ch = 0;
        for(i=0; i<8; i++){
            ch+=(fgetc(in)&0x01)<<i;    //读取一个字符，和0x01做按位与运算，取其末位，移位后放进ch中
            for(r=0; r<random; r++){
                fseek(in,1L,1);
            }   //每提取一位，就执行random次fseek函数，跳过random个字节
        }
        putchar(ch);    //输出这个字符
    }while(ch);
    printf("\nPicking up over.\n");
}