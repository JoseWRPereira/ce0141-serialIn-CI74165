/*
 * File:   serialOut.c
 * Author: curtaeletronica
 *
 * Created on 30 de Setembro de 2019, 18:44
 * 
 * Objetivo: 
 *      Camada de Abstração de Hardware:
 *          Interface com o CI 74165 (Shift Register).
 * 
 * Pinos    |nº     |Conexão
 * RE0      |8      | SHLD : Shift / Load
 * RE1      |9      | CLK : Clock
 * RE2      |10     | IN
 */

//***************** Bibliotecas
#include <xc.h>
#include "config.h"


//***************** Interface com PORTs/Pinos
#define SERIALIO_ADDRS 0x009       // PORTE

//***************** Structs
typedef union
{
    struct
    {
        unsigned char SHLD:1;
        unsigned char CLK :1;
        unsigned char IN :1;
    };
} SERIALIObits_t;

volatile SERIALIObits_t SERIALIO __at(SERIALIO_ADDRS);



/****************** Procedimento initSerialOut
 * Entrada: -
 * Retorno: -
 * Objetivo: Inicializar a interface com o CI 74165
 */
void initSerialIn( void )
{
//***************** Serial IO Shift/Load
    PORTEbits.RE0 = 0;
    ANSELbits.ANS5 = 0;
    TRISEbits.TRISE0 = 0;

// **************** Serial IO Clock
    PORTEbits.RE1 = 0;
    ANSELbits.ANS6 = 0;
    TRISEbits.TRISE1 = 0;

// **************** Serial IO Input
    PORTEbits.RE2 = 0;
    ANSELbits.ANS7 = 0;
    TRISEbits.TRISE2 = 1;

    SERIALIO.CLK = 0;
    SERIALIO.IN = 0;
    SERIALIO.SHLD = 1;
    
}


/****************** Procedimento serialIn
 * Entrada: -
 * Retorno: Dado a ser recebido.
 * Objetivo: Lê dado de forma serial via o registrador de 
 *          deslocamento 74165, que o converte paralelo em serial. 
 */
unsigned char serialIn( void )
{
    unsigned char i;
    unsigned char dado = 0;
    
    SERIALIO.SHLD = 0;
    i = 0x80;
    SERIALIO.SHLD = 1;
    while( i )
    {
        SERIALIO.CLK = 0;
        if( SERIALIO.IN )
            dado |= i;
        SERIALIO.CLK = 1;
        i >>= 1;
    }
    return( dado );
}
