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
    random=rand()%10+1; //����һ���������Ϊ�˲��Է��㣬�˴�ʹ��1��10֮��������
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
    int i,j,m,n,r;  //������
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
            //�ѵ�m��λ�õ��ַ���ÿһλ����ȡ��������λ���0x01����λ�����㣩
        }
    }
    if((in=fopen(carrierFile,"rb"))==NULL){
        printf("Carrier file fopen error.\n");
        return;
    }   //�������ļ�
    if((out=fopen(outputFile,"wb"))==NULL){
        printf("Output file fopen error.\n");
        return;
    }   //������ļ�
    for(i=0,j=0; !feof(in); i++){
        if(i<54){
            fputc(fgetc(in),out);   //ǰ54���ַ�Ϊbmp�ļ�ͷ������
        }
        else
        {
            if(j<8*strlen(info)){
                //��һ���ַ�����0xfe����λ�����㣬����ĩλ��LSB������
                fputc((fgetc(in)&0xfe) + temp[j], out);
                for(r=0; r<random; r++){
                    fputc(fgetc(in)&0xfe, out);
                }   //��fgetc��������random���ַ�
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
    unsigned int i,r;   //������
    char FName[90], ch;
    printf("File to pickup: ");
    scanf("%s",FName);
    if((in=fopen(FName,"rb"))==NULL){
        printf("File fopen error.\n");
        return;
    }   //��Ҫ��ȡ���ļ�
    fseek(in,54L,0);    //ʹ��fseek����54�ֽڵ�ͷ
    printf("Secret information: ");
    do{
        ch = 0;
        for(i=0; i<8; i++){
            ch+=(fgetc(in)&0x01)<<i;    //��ȡһ���ַ�����0x01����λ�����㣬ȡ��ĩλ����λ��Ž�ch��
            for(r=0; r<random; r++){
                fseek(in,1L,1);
            }   //ÿ��ȡһλ����ִ��random��fseek����������random���ֽ�
        }
        putchar(ch);    //�������ַ�
    }while(ch);
    printf("\nPicking up over.\n");
}