#ifndef IMAGETYPE_H
#define IMAGETYPE_H


typedef struct Thrddouble
{
    double varH;
    double varS;
    double varO;
}thrddouble,*pthrdfloat;
#define DOTISO_MAC(R1,G1,B1,d_h,d_s,d_o)\
{\
    int max[3];\
    int temp;\
    char Smax[3];\
    char stemp;\
    max[0]=R1;\
    max[1]=B1;\
    max[2]=G1;\
    Smax[0]='R';\
    Smax[1]='B';\
    Smax[2]='G';\
    if(max[1]>max[0])\
{\
    temp=max[0];\
    max[0]=max[1];\
    max[1]=temp;\
    stemp=Smax[0];\
    Smax[0]=Smax[1];\
    Smax[1]=stemp;\
}\
    if(max[2]>max[1])\
{\
    temp=max[1];\
    max[1]=max[2];\
    max[2]=temp;\
    stemp=Smax[1];\
    Smax[1]=Smax[2];\
    Smax[2]=stemp;\
}\
    if(max[1]>max[0])\
{\
    temp=max[0];\
    max[0]=max[1];\
    max[1]=temp;\
    stemp=Smax[0];\
    Smax[0]=Smax[1];\
    Smax[1]=stemp;\
}\
    int iH = (max[0]+max[1]+max[2]);\
    double H=intx0_5(iH) +intx0_062(iH) +intx0_015(iH);\
    int snum=(max[1]*max[1]+max[2]*max[2]+max[0]*max[0]-max[0]*max[2]-max[1]*max[2]-max[1]*max[0]);\
    double S=ISOsqrtX[snum];\
    int S1=(((max[0])<<1)-(max[1]+max[2]));\
    int S2=(max[1]-max[2]);\
    double a=ISOAtanAngle[S1][S2];\
    a=(a-30)>0?(a-30):0;\
    if(Smax[0]=='R')\
{\
    if(Smax[1]=='B')\
    a=a+180;\
        else\
        a=300-a;\
}\
        else\
{\
    if(Smax[0]== 'B')\
{\
    if(Smax[1]=='G')\
    a=a+60;\
        else \
        a=180-a;\
}\
        else\
{\
    if(Smax[0]=='G')\
{\
    if(Smax[1]=='R')\
    a=a+300;\
        else \
        a=60-a;\
}\
        else\
{\
    if(0);\
}\
}\
}\
    d_h=H;\
    d_s=S;\
    d_o=a;\
}
#define  intx0_5(i) (i>>1)
#define  intx0_25(i) (i>>2)
#define  intx0_125(i) (i>>3)
#define  intx0_062(i) (i>>4)
#define  intx0_031(i) (i>>5)
#define  intx0_015(i) (i>>6)
#define  intx0_007(i) (i>>7)
#define  intx0_003(i) (i>>8)
#define  intx0(i) (i>>9)
#define  intx2(i) (i<<1)
#define  intx4(i) (i<<2)
#define  intx8(i) (i<<3)
#define  intx16(i) (i<<4)
#define  intx256(i) (i<<8)

#define  intx0_5(i) (i>>1)
#define  intx0_25(i) (i>>2)
#define  intx0_125(i) (i>>3)
#define  intx0_062(i) (i>>4)
#define  intx0_031(i) (i>>5)
#define  intx0_015(i) (i>>6)
#define  intx0_007(i) (i>>7)
#define  intx0_003(i) (i>>8)
#define  intx0(i) (i>>9)
#define  intx2(i) (i<<1)
#define  intx4(i) (i<<2)
#define  intx8(i) (i<<3)
#define  intx16(i) (i<<4)
#define  intx256(i) (i<<8)


//Gauss 9
#define  MATRIXNUM   9

static const float GausPar0[MATRIXNUM]
={
    //	0.0625,0.125,0.1875,0.5,0.1875,0.125,0.0625
    //	0.02,0.08,0.125,0.15,0.25,0.15,0.125,0.08,0.02
    0 ,0,0 ,0 ,1 ,0,0 ,0 ,0
};
static const int GausPar0_INT[MATRIXNUM]
={
    //	0.0625,0.125,0.1875,0.5,0.1875,0.125,0.0625
    //	0.02,0.08,0.125,0.15,0.25,0.15,0.125,0.08,0.02
    0 ,0,0 ,0 ,1 ,0,0 ,0 ,0
};
static const double GausPar[MATRIXNUM]
={
//	0.0625,0.125,0.1875,0.5,0.1875,0.125,0.0625
//	0.02,0.08,0.125,0.15,0.25,0.15,0.125,0.08,0.02
    0.01,0.06,0.125,0.15,0.31,0.15,0.125,0.06,0.01
};

//static const int GausPar_INT[MATRIXNUM]//<<8
//={
    //	0.0625,0.125,0.1875,0.5,0.1875,0.125,0.0625
    //	0.02,0.08,0.125,0.15,0.25,0.15,0.125,0.08,0.02
    //0.01*256,0.06*256,0.125*256,0.15*256,0.31*256,0.15*256,0.125*256,0.06*256,0.01*256
//};


static const double GausPar_Smoth[MATRIXNUM]
={
     0.02,0.08,0.125,0.15,0.25,0.15,0.125,0.08,0.02
 };

//static const int  GausPar_Smoth_INT[MATRIXNUM]//<<8
//={
//    0.02*256,0.08*256,0.125*256,0.15*256,0.25*256,0.15*256,0.125*256,0.08*256,0.02*256
//};
static const double GausPar_moreSmoth[MATRIXNUM]
={
    0.02,0.105,0.135,0.16,0.20,0.16,0.135,0.105,0.02
};
//static const int GausPar_moreSmoth_INT[MATRIXNUM]
//={
//    0.02*256,0.105*256,0.135*256,0.16*256,0.20*256,0.16*256,0.135*256,0.105*256,0.02*256
//};
static const double GausPar_average[MATRIXNUM]
={
    0.112,0.112,0.112,0.112,0.112,0.112,0.112,0.112,0.112
};

//static const int GausPar_average_INT[MATRIXNUM]
//={
//    0.112*256,0.112*256,0.112*256,0.112*256,0.112*256,0.112*256,0.112*256,0.112*256,0.112*256
//};
static const int GausMatix[MATRIXNUM]
={
    -4,-3,-2,-1,0,1,2,3,4
};

#define  MATRIXNUM5   5
    const double Gaus5Par[MATRIXNUM5]
    ={
        0.13 ,0.21,0.32,0.21,0.13
        //intx0_125(i-2) ,intx0_25(i-1),intx0_25(i)+intx0_062(i),intx0_25(i+1),intx0_125(i+2)
    };

    const double Gaus5Par_Smoth[MATRIXNUM5]
    ={
         0.14,0.22,0.28,0.22,0.14
    };
    const double Gaus5Par_moreSmoth[MATRIXNUM5]
    ={
        0.15,0.22,0.26,0.22,0.15
    };
    const double Gaus5Par_average[MATRIXNUM5]
    ={
        0.2,0.2,0.2,0.2,0.2
    };
#endif // imagetype_H
