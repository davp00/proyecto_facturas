#include <iostream>
#include <windows.h>
#include <conio.h>
#include <sstream>
#define TECLA 48

using namespace std;


struct Fecha
{
    int day;
    int month;
    int year;

    Fecha( string _day, string _month, string _year )
    {
        datos(_day, _month, _year);
    }

    void datos( string _day, string _month, string _year )
    {
        day     = ( _day.size() != 0 && _day.size() <= 2 )? atoi( _day.c_str() ): 0;
        month   = ( _month.size() != 0 && _month.size() <= 2 )? atoi( _month.c_str() ): 0;
        year    = ( _year.size() != 0 && _year.size() <= 4 )? atoi( _year.c_str() ) : 0;
    }

    bool bisiesto()
    {
        return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
    }


    bool mes30()
    {
        return month == 4 || month == 6 || month == 9 || month == 11;
    }

    bool igual ( Fecha *fecha )
    {
        return day == fecha->day && month == fecha->month && year == fecha->year;
    }

    bool despues_de( Fecha *fecha )
    {
        return day >= fecha->day && month >= fecha->month && year >= fecha->year;
    }

    bool antes_de( Fecha *fecha )
    {
        return day <= fecha->day && month <= fecha->month && year <= fecha->year;
    }

    bool valida()
    {
        if ( day == 0 || month == 0 || year == 0)
            return false;

        if ( month == 2 )
        {
            if ( bisiesto() && day <= 29 )
                return true;
            else
                return day <= 28;
        }else
            if ( mes30() )
                return day <= 30;
            else
                return day <= 31;
    }

    string toString()
    {
        ostringstream temp;
        temp << day << "/" << month << "/" << year;

        string date = temp.str();

        return date;
    }
};

struct Factura
{
    int num;
    Fecha *fecha;
    string cliente;

    Factura *sig;
};

struct FacturaInfo
{
    int numFactura;

    string producto;
    int cant;
    int valor_unitario;

    FacturaInfo *sig;
};

char getcht ( );
string leer_letras( );
string leer_numeros( );
void menus(int tipo);
int menu(int ini , int fin , int tipo);
void gotoxy( int x, int y );

Factura *insertar_factura( Factura *ptr, int num, Fecha *fecha, string cliente );
int N_facturas( Factura *ptr );

FacturaInfo *insertar_facturaInfo( FacturaInfo *ptr, int numFactura, string producto, int cant, int valor_unitario );
bool existe_producto( FacturaInfo *ptr, string producto );


void crear_factura();

Factura *facturas   = NULL;
FacturaInfo *infos   = NULL;


Factura *fct = NULL;
FacturaInfo *fct_info = NULL;

int main ( )
{
    bool salir = false;

    do
    {

        switch( menu(1+TECLA,4+TECLA, 1) )
        {
            case 1 + TECLA:
                system("CLS");
                crear_factura();
                getch();
            break;

            case 2 + TECLA:

            break;

            case 3 + TECLA:

            break;

            case 4 + TECLA:

            break;

            case 27:
                salir = true;
            break;
        }

    }while ( ! salir );



	return 0;
}



void crear_factura()
{
    string cliente;
    string day;
    string month;
    string year;

    fct         = new Factura();
    fct->num    = N_facturas( facturas ) + 1;

    menus(2);
    gotoxy(51, 3);  cout << fct->num;
    gotoxy(42, 8);  fct->cliente= leer_letras();
    gotoxy(45, 9);  day         = leer_numeros();
    gotoxy(45, 10); month       = leer_numeros();
    gotoxy(45, 11); year        = leer_numeros();

    fct->fecha = new Fecha( day , month, year );

    if ( fct->fecha->valida() )
    {
        bool salir = false;

        do
        {
            system("CLS");
            gotoxy(10, 2);cout << "Factura # "<< fct->num ;
            gotoxy(10, 4);cout << "Cliente: " << fct->cliente;
            gotoxy(10, 5);cout << "Fecha: "<< fct->fecha->toString();

            switch( menu( 1+TECLA, 3+TECLA, 4 ) )
            {
                case 3 + TECLA:
                    salir = true;
                    system("CLS");
                break;
            }
        }while( ! salir );
    }else
    {
        gotoxy(30, 17); cout << "ERROR: La fecha no es valida";
    }

}


Factura *insertar_factura( Factura *ptr, int num, Fecha *fecha, string cliente )
{
    Factura *nueva  = new Factura();
    nueva->num      = num;
    nueva->fecha    = fecha;
    nueva->cliente  = cliente;

    nueva->sig      = NULL;

    if ( ptr )
    {
        ptr = nueva;
    }else
    {
        Factura *ant = ptr;
        while ( ant->sig != NULL ) ant = ant->sig;
        ant->sig = nueva;
    }

    return ptr;
}

int N_facturas( Factura *ptr )
{
    Factura *i = ptr;
    int cont = 0;

    while ( i )
    {
        cont ++;
        i = i->sig;
    }

    return cont;
}

FacturaInfo *insertar_facturaInfo( FacturaInfo *ptr, int numFactura, string producto, int cant, int valor_unitario )
{
    FacturaInfo *nueva      = new FacturaInfo();
    nueva->numFactura       = numFactura;
    nueva->producto         = producto;
    nueva->cant             = cant;
    nueva->valor_unitario   = valor_unitario;
    nueva->sig              = NULL;


    if ( ! ptr )
    {
        nueva = ptr;
    } else
    {
        FacturaInfo *ant = ptr;
        while( ant->sig != NULL ) ant = ant->sig;
        ant->sig = nueva;
    }

    return ptr;
}

int menu(int ini , int fin , int tipo)
{
	int tecla;
	bool error=true;

	int intentos=0;
    menus(tipo);
	do{
	tecla=getch();

		if(tecla>=ini && tecla<=fin || tecla==27)
        {
            error=false;
            return tecla;
		}else{
			intentos++;
			gotoxy(30,20+intentos);cout<<("Porfavor seleccione un numero del menu");
			if(intentos == 4){
                    intentos=0;
					menus(tipo);
			}
		}

	}while(error);
}


void menus(int tipo)
{
    system("Color 4F");

	if(tipo == 1)
    {
        gotoxy(30,2);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");
        gotoxy(30,3);cout << ("²                  HUD                    ²");
        gotoxy(30,4);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");
        gotoxy(30,5);cout << ("²          Sistema de medallas            ²");
        gotoxy(30,6);cout << ("²                                         ²");
        gotoxy(30,7);cout << ("²    1-> Crear Factura                    ²");
        gotoxy(30,8);cout << ("²    2-> Mostrar Factura                  ²");
        gotoxy(30,9);cout << ("²    3-> Editar Factura                   ²");
        gotoxy(30,10);cout << ("²    4-> Informe por fecha                ²");
        gotoxy(30,11);cout << ("²    5-> Informe por rango de fecha       ²");
        gotoxy(30,12);cout << ("²                                         ²");
        gotoxy(30,13);cout << ("²                                         ²");
        gotoxy(30,14);cout << ("²        ESC-> Salir                      ²");
        gotoxy(30,15);cout << ("²                                         ²");
        gotoxy(30,16);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");
    }


    if(tipo == 2)
    {
        gotoxy(30,2);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");
        gotoxy(30,3);cout << ("²          Factura #             ²");
        gotoxy(30,4);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");
        gotoxy(30,5);cout << ("²           Buscar               ²");
        gotoxy(30,6);cout << ("²                                ²");
        gotoxy(30,7);cout << ("²                                ²");
        gotoxy(30,8);cout << ("²  Cliente:                      ²");
        gotoxy(30,9);cout << ("²  Fecha (DD):                   ²");
        gotoxy(30,10);cout << ("²  Fecha (MM):                   ²");
        gotoxy(30,11);cout << ("²  Fecha (YY):                   ²");
        gotoxy(30,12);cout << ("²                                ²");
        gotoxy(30,13);cout << ("²                                ²");
        gotoxy(30,14);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");
    }

      if(tipo == 3)
    {
        gotoxy(30,2);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");
        gotoxy(30,3);cout << ("²           Equipos              ²");
        gotoxy(30,4);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");
        gotoxy(30,5);cout << ("²           Puntos               ²");
        gotoxy(30,6);cout << ("²                                ²");
        gotoxy(30,7);cout << ("²  De:                           ²");
        gotoxy(30,8);cout << ("²  Hasta:                        ²");
        gotoxy(30,9);cout << ("²                                ²");
        gotoxy(30,10);cout << ("²                                ²");
        gotoxy(30,11);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");
    }

    if( tipo  == 4 )
    {
        gotoxy(50,2);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²");
        gotoxy(50,3);cout << ("² Menu                 ²");
        gotoxy(50,4);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²");
        gotoxy(50,5);cout << ("² 1-> Agregar producto ²");
        gotoxy(50,6);cout << ("² 2-> Confirmar        ²");
        gotoxy(50,7);cout << ("²                      ²");
        gotoxy(50,8);cout << ("² 3-> Salir            ²");
        gotoxy(50,9);cout << ("²                      ²");
        gotoxy(50,10);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²");
    }

}


string leer_letras()
{
    int i = 0;
    char tecla;
    char *p = new char();



    p[0] = '\0';
    do
    {
        tecla = getcht ();

        if ( i > 0 && tecla == 8 )
        {
            cout << ( "\b \b" );
            p[--i] = '\0';
        }
        else
        {
            /*rango de letras mayusculas y minusculas y 32 es el espacio*/
            if ( (tecla >= 65 && tecla <= 95)  ||
                 (tecla >= 97 && tecla <= 122) ||
                  tecla == 32 )
            {
                cout << tecla ;
                p[i++] = tecla;
            }
        }
    } while ((tecla != 13 || p[0] == '\0'));
    /* 13 inidica el Enter, i tiene que ser menor
    que TAM pues de lo contrario, estarÃ­a accediendo
    a memoria no reservada*/

    p[i] = '\0'; /*colocamos el fin de cadena*/

    return string( p );
}


string leer_numeros()
{
    int i;
    char tecla;
	char *p = new char();
    /*importante inicializar los valores
    ya que la implementacion que hice lo amerita*/
    i = 0;
    p[0] = '\0';

    do
    {
        tecla = getcht ();//atrapa un caracter sin dejar un eco

        /*8 es la tecla para eliminar un caracter
        y el i obligaotiamente tiene que ser mayor que cero
        ya que si entra al if y tiene valor cero, el valor de i
        decrementarï¿½ y p tendrï¿½a un subindice -1, "p[-1]"ï¿½?*/
        if ( i > 0 && tecla == 8 )
        {
            cout << ( "\b \b" );
            p[--i] = '\0';
        }
        else
        {
            /*rango de letras mayusculas y minusculas y 32 es el espacio*/
            if ( (tecla >= 48 && tecla <= 57) )
            {
                cout << tecla;
                p[i++] = tecla;
            }
        }
    } while ( (tecla != 13 || p[0] == '\0') );
    /* 13 indica el Enter, i tiene que ser menor
    que TAM pues de lo contrario, estarï¿½a accediendo
    a memoria no reservada*/

    p[i] = '\0'; /*colocamos el fin de cadena*/

    return string( p );
}


void gotoxy(int x , int y)
{
    HANDLE manipulador;
    COORD Coordenada;

    manipulador = GetStdHandle(STD_OUTPUT_HANDLE);
    Coordenada.X = x;
    Coordenada.Y = y;

    SetConsoleCursorPosition(manipulador,Coordenada);

}


char getcht ( )
{
    char car;
    DWORD leidos, modo;

    GetConsoleMode ( GetStdHandle (STD_INPUT_HANDLE), &modo );
    SetConsoleMode ( GetStdHandle (STD_INPUT_HANDLE), modo &
                     !ENABLE_ECHO_INPUT & !ENABLE_PROCESSED_INPUT );
    ReadConsole ( GetStdHandle (STD_INPUT_HANDLE), &car, 1, &leidos, NULL );
    SetConsoleMode ( GetStdHandle (STD_INPUT_HANDLE), modo );
    return car;
}



