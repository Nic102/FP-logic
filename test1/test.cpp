
#include <iostream>
#include <string>//++ строки нужно
#include <ctime>//-+ таймер
#include <math.h>//+ степень и log
#include <fstream>// файлы

using namespace std;

string infnan;
unsigned int search_time = 0;

void in(unsigned long long& num)
{
    char pref;
        cin >> pref;

    if (pref == 'b')
    {
        string bin;
        cin >> bin;
        num = stoull(bin, 0, 2);
    }
    else if (pref == 'h')
    {
        string hex;
        cin >> hex;
        num = stoull(hex, 0, 16);
    }
}


void Sum(long long a, long long b, long long& c, short expLen, short manLen)
{
    long long temporary = 0, otvsign = 0, otvexp = 0, otv = 0, tailA = 0, tailB = 0, tail = 0;
    int count = 0;
    short int signA, signB;
    if (a>(0x7FFFFFFFFFFFFFFF >> (63 - (expLen + manLen))))
        signA = 1;
    else
        signA = 0;

    int expA = ((a >> manLen) & (0xffff >> (16 - expLen)));
    long long manA = a & (0x7FFFFFFFFFFFFFFF >> (63 - manLen));

    if (b > (0x7FFFFFFFFFFFFFFF >> (63 - (expLen + manLen))))
        signB = 1;
    else
        signB = 0;

    int expB = ((b >> manLen) & (0xffff >> (16 - expLen)));
    long long manB = b & (0x7FFFFFFFFFFFFFFF >> (63 - manLen));

    if (((expA == (pow(2, expLen) - 1) and manA == 0) and (signA == 0 or signA == 1)) or ((expB == (pow(2, expLen) - 1) and manB == 0) and (signB == 0 or signB == 1)))
        cout << "";
    else if (((expA == (pow(2, expLen) - 1) and manA > 0) and (signA == 0 or signA == 1)) or ((expB == (pow(2, expLen) - 1) and manB > 0) and (signB == 0 or signB == 1)))
        cout << "";
    else
    { 
        if ((expA - expB > manLen) and expA > expB)
        {
            c = manA;
            otvexp = expA;
            otvsign = signA;
        }
        else if ((expB - expA > manLen) and expA < expB)
        {
            c = manB;
            otvexp = expB;
            otvsign = signB;
        }
        else
        {
            manA |= 0b1ll << manLen;
            manB |= 0b1ll << manLen;

            if (expA > expB)
            {
                tailB = (tailB >> (expA - expB)) | (manB << (64 - (expA - expB)));
                manB >>= (expA - expB);
                otvexp = expA;
            }
            else if (expA < expB)
            {
                tailA = (tailA >> (expB - expA)) | (manA << (64 - (expB - expA)));
                manA >>= (expB - expA);
                otvexp = expB;
            }
            else if (expA == expB)
                otvexp = expB;

            if ((manA > manB) and (signA == 1))
                otvsign = 1;
            else if ((manA < manB) and (signB == 1))
                otvsign = 1;
            else if (manA == manB and signA != signB)
                otvsign = 0;
            else
                otvsign = 0;

            if (signA != signB)
            {
                if (manA < manB)
                {
                    manA = ~manA;
                    tailA = ~tailA;
                }
                else if (manA > manB)
                {
                    manB = ~manB;
                    tailB = ~tailB;
                }

                c = manA + manB;
                tail = (tailA + tailB) + 1;

                if (tail == 0)
                    c += 1;
            }
            else
            {
                c = manA + manB;
                tail = tailA + tailB;
            }

            int index2 = 0;
            index2 = log(c) / log(2);

            if (manLen > index2)
            {
                int razn = 0;
                bool razn_otvexp = 0;
                short razn1 = 0;

                razn = manLen - index2;
                if (razn > otvexp)
                {
                    razn_otvexp = 1;
                    razn = otvexp - 1;
                }
                c <<= razn;

                if (expA < expB)
                    razn1 = razn + 1;
                else
                    razn1 = razn;

                c |= (tail >> (64 - razn)) & (0x7FFFFFFFFFFFFFFF >> (64 - razn1));

                tail <<= razn;

                if (otvexp - razn < 0)
                    otvexp = 0;
                else if (razn_otvexp == 1)
                    otvexp -= razn + 1;
                else
                    otvexp -= razn;
            }

            if (c >= (0b1ll << (manLen + 1)))
            {
                c >>= 1;
                otvexp++;
            }

            if ((((c & 1) == 1) and ((tail & 0x8000000000000000) == 0x8000000000000000)) or (((c & 1) == 0) and (tail > 0x8000000000000000)))
                c++;
        }

        long long otvmant = c;//проверка

        c &= ~(0b1ll << manLen);
        c |= (otvexp << manLen);
        c |= (otvsign << manLen + expLen);

        if ((otvexp == (pow(2, expLen) - 1) and otvmant == 0) and (otvsign == 0 or otvsign == 1))
            infnan = "inf";
        else if ((otvexp == (pow(2, expLen) - 1) and otvmant > 0) and (otvsign == 0 or otvsign == 1))
            infnan = "nan";
        else
            infnan = "ch";
    }
}


void Nach(unsigned long long num1, unsigned long long num2, long long& num33)
{
    short expLen = 8, manLen = 23;//|8,23|,|11,52|,|8,10|

    if(num1==0 and num2==0 and num33==1)
    {
        in(num1);
        in(num2);
    }

    Sum(num1, num2, num33, expLen, manLen);
}


void inFile()
{
    unsigned long long num1 = 0, num2 = 0;
    long long num3 = 0;
    ifstream file_A;
    ifstream file_B;
    file_A.open("32b1.txt");
    file_B.open("32b2.txt");
    string buf;

    while (getline(file_A, buf))
    {
        num1 = stoull(buf, 0, 16);
        getline(file_B, buf);
        num2 = stoull(buf, 0, 16);

        unsigned int start_time = clock();
        Nach(num1, num2, num3);
        unsigned int end_time = clock();
        search_time += end_time - start_time;
    }
}


int main()
{
    unsigned long long num1 = 0, num2 = 0;
    long long num3 = 1;
    infnan = "";
    //inFile();//ввод через файл
    unsigned int start_time1 = clock();
    Nach(num1, num2, num3);//ввод вручную
    unsigned int end_time1 = clock();
    if(search_time==0)
        search_time = end_time1 - start_time1;

    if (infnan == "inf" or infnan=="nan")// только для ввода в ручную если через файл то закоментить 
        cout << infnan <<endl;
    else if(infnan=="ch")
        cout << hex << num3 << endl;

    std::cout << "                      Clock ------ " << search_time / 1000.0 << " ------ " << endl << endl;
    //return main();// только для ввода в ручную если через файл то закоментить  
}
