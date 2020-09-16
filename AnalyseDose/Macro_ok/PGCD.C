#include <stdio.h>
#include <stdlib.h>
 
 
 int PGCD_Macro (int f_firstNumber,int f_secondNumber)
{
    if (f_firstNumber == 0 and f_secondNumber == 0)//Pour le cas PGCD(0;0) et pour éviter plantage du logiciel
    {
        return 0;
    }
 
    int resultatEuclidien (0);
 
    while (f_secondNumber != 0)
    {
        resultatEuclidien = f_firstNumber % f_secondNumber;
 
        f_firstNumber = f_secondNumber;
        f_secondNumber = resultatEuclidien;
    }
 
    return f_firstNumber;
}
 
void PGCD()
{
    long nombrea = 0; //On initialise les variables à 0
    long nombreb = 0;
    long reste = 1;     // La boucle ne s'activera pas si le reste est déjà égale à 0
    long reste2 = 1;
    long quotient = 0;
    long PGCD = 0;
 
    printf("Le PGDC\n");
    printf("\n\n\n Tapez les nombres pour trouver leurs PGCD\n\n");
    printf("Tout d'abord le premier nombre (Entier positif seulement)\n");
    scanf("%ld", &nombrea);
    printf("Et le deuxieme nombre\n\n");
    scanf("%ld", &nombreb);
 
//     int num, result=0;
//    scanf("%d", &num);
//    while (num>0){
//     result = result + num%10;
//    num = num/10;
//    }
//     printf("le resultas est :%d", result);
    
    
    
    
   int NbrChiffres = (int)(1+log10(nombrea));
   char str[NbrChiffres];
   
    sprintf(str, "%ld", nombrea);
    
    string chiffre; 
    int nbr0 = 0;
    
    for(int i = 0;i < NbrChiffres ; i++){
    
        string ala;
        ala.push_back(str[i]);
        
        if(ala != "0")chiffre += str[i];
        else nbr0++;
        
        
    }
        cout << nombrea << " nb"<< NbrChiffres << endl;
    cout << chiffre << " 10^"<< nbr0 << endl;
    
    
// str => {'1', '5', '\0', x, x}
    
    
    
//     cout << PGCD_Macro(nombrea,nombreb) << endl;
}
