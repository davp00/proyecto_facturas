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

    int limite()
    {
        if ( month == 2 )
            return bisiesto() ? 29 : 28;
        else
            if ( mes30() )
                return 30;
            else
                return 31;
    }

    void aumentar()
    {
        if ( day < limite() )
            day ++;
        else
        {
            day = 1;

            if ( month == 12 )
            {
                month   = 1;
                year++;
            }else
            {
                month ++;
            }
        }
    }

    bool igual ( Fecha fecha )
    {
        return day == fecha.day && month == fecha.month && year == fecha.year;
    }

    bool despues_de( Fecha fecha )
    {
        return day >= fecha.day && month >= fecha.month && year >= fecha.year;
    }

    bool antes_de( Fecha fecha )
    {
        return day <= fecha.day || month < fecha.month || year < fecha.year;
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
    Fecha fecha;
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

Factura *insertar_factura( Factura *ptr, int num, Fecha fecha, string cliente );
int N_facturas( Factura *ptr );
Factura *buscar_factura( Factura *ptr, int num );
Factura *facturas_por_fecha( Factura *ptr, Fecha fecha);

FacturaInfo *insertar_facturaInfo( FacturaInfo *ptr, int numFactura, string producto, int cant, int valor_unitario );
bool existe_producto( FacturaInfo *ptr, string producto );
FacturaInfo *buscar_producto( FacturaInfo *ptr, int numFactura ,string producto);
FacturaInfo *info_facturas( FacturaInfo *ptr, int numFactura );
int totalFactura( int numFactura );


void crear_factura();
void mostrar_factura();
void informe_por_fecha();
void informe_por_rango();
void editar_factura();

Factura *facturas   = NULL;
FacturaInfo *infos   = NULL;


Factura *fct = NULL;
FacturaInfo *fct_info = NULL;

int main ( )
{
    bool salir = false;


    do
    {

        switch( menu(1+TECLA,5+TECLA, 1) )
        {
            case 1 + TECLA:
                system("CLS");
                    crear_factura();
            break;

            case 2 + TECLA:
                system("CLS");
                    if( facturas != NULL )
                        mostrar_factura();
                    else
                        cout<< "No existen facturas en los registros";
                getch();
                system("CLS");
            break;

            case 3 + TECLA:
                system("CLS");
                    if ( facturas )
                        editar_factura();
                    else
                    {
                        cout << "No hay facturas para editar";
                        getch();
                    }

                system("CLS");
            break;

            case 4 + TECLA:
                system("CLS");
                    if( facturas != NULL )
                        informe_por_fecha();
                    else
                        cout<< "No existen facturas en los registros";
                getch();
                system("CLS");
            break;

            case 5 + TECLA:
                system("CLS");
                    if ( facturas )
                        informe_por_rango();
                    else
                        cout<< "No existen facturas en los registros";
                getch();
                system("CLS");
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

    fct->fecha.datos( day , month, year );

    if ( fct->fecha.valida() )
    {
        bool salir = false;

        do
        {
            system("CLS");
            gotoxy(10, 2);cout << "Factura # "<< fct->num ;
            gotoxy(10, 4);cout << "Cliente: " << fct->cliente;
            gotoxy(10, 5);cout << "Fecha: "<< fct->fecha.toString();


            gotoxy(10, 12);cout << "___________________________________________________________________";
            gotoxy(10, 14);cout << "Producto";
            gotoxy(30, 14);cout << "Cantidad";
            gotoxy(45, 14);cout << "Valor U.";
            gotoxy(60, 14);cout << "Total $";

            FacturaInfo *i = fct_info;
            int y = 16;
            int total = 0;
            int totalProducto = 0;

            while ( i != NULL )
            {
            	gotoxy(10, y); cout << i->producto;
            	gotoxy(30, y); cout << i->cant;
            	gotoxy(45, y); cout << i->valor_unitario;

            	totalProducto 	= i->cant * i->valor_unitario;
            	total 			+= totalProducto;

            	gotoxy(60, y); cout << totalProducto;
            	y ++;
            	i = i->sig;
			}

			gotoxy(10, y+1);cout << "___________________________________________________________________";
			gotoxy(52, y+3);cout << "Total : " << total;

            switch( menu( 1+TECLA, 3+TECLA, 4 ) )
            {
                case 1 + TECLA:
				{
					string producto;
					int cantidad;
					int valor_unitario;

					system("CLS");
            		menus( 3 );
            		gotoxy(40, 7); producto = leer_letras();


            		if( existe_producto( fct_info, producto ) )
                    {
                        gotoxy(10, 15); cout << "Ya existe un producto con este nombre en la factura";
                        getch();
                        break;
                    }

            		gotoxy(43, 8); cantidad	= atoi( leer_numeros().c_str() );
            		gotoxy(48, 9); valor_unitario = atoi( leer_numeros().c_str());

            		fct_info = insertar_facturaInfo( fct_info, fct->num, producto, cantidad, valor_unitario );
				}
                break;


                case 2 + TECLA:
            	{
            		if ( fct_info != NULL )
            		{
            			facturas = insertar_factura( facturas, fct->num, fct->fecha, fct->cliente);


            			while ( fct_info != NULL )
            			{
            				infos	= insertar_facturaInfo( infos, fct->num, fct_info->producto, fct_info->cant, fct_info->valor_unitario);
            				fct_info= fct_info->sig;
						}

						gotoxy(10, y+5); cout << "Factura guardada con exito";
                        getch();
						fct_info    = NULL;
						fct         = NULL;
						salir 		= true;
						system("CLS");
					}else
					{
						gotoxy(10, y+5);cout << "Debe haber por lo menos un producto en la factura";
						getch();
					}
				}
				break;

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


void mostrar_factura()
{
    int n_factura;

    menus(5);
    gotoxy(40, 7); n_factura = atoi( leer_numeros().c_str() );


    system("CLS");

    Factura *factura = buscar_factura( facturas, n_factura );


    if( factura == NULL )
    {
        gotoxy(10, 2 ); cout << "El numero de factura no fue encontrado";
        return ;
    }

    gotoxy(10, 2);cout << "Factura # "<< factura->num ;
    gotoxy(10, 4);cout << "Cliente: " << factura->cliente;
    gotoxy(10, 5);cout << "Fecha: "<< factura->fecha.toString();


    gotoxy(10, 12);cout << "___________________________________________________________________";
    gotoxy(10, 14);cout << "Producto";
    gotoxy(30, 14);cout << "Cantidad";
    gotoxy(45, 14);cout << "Valor U.";
    gotoxy(60, 14);cout << "Total $";

    FacturaInfo *i = info_facturas( infos, factura->num);
    int y = 16;
    int total = 0;
    int totalProducto = 0;

    while ( i != NULL )
    {
        gotoxy(10, y); cout << i->producto;
        gotoxy(30, y); cout << i->cant;
        gotoxy(45, y); cout << i->valor_unitario;

        totalProducto 	= i->cant * i->valor_unitario;
        total 			+= totalProducto;

        gotoxy(60, y); cout << totalProducto;
        y ++;
        i = i->sig;
    }


    gotoxy(10, y+1);cout << "___________________________________________________________________";
    gotoxy(52, y+3);cout << "Total : " << total;
}



void editar_factura()
{
    int n_factura;

    menus( 8 );

    gotoxy(40, 7); n_factura = atoi( leer_numeros().c_str() );
    system("CLS");


    Factura *factura = buscar_factura( facturas, n_factura );

    if ( factura == NULL )
    {
        gotoxy(10,2); cout << "Factura no encontrada";
        return;
    }


    bool salir = false;

    system("CLS");

    do
    {

        gotoxy(10, 2);cout << "Factura # "<< factura->num ;
        gotoxy(10, 4);cout << "Cliente: " << factura->cliente;
        gotoxy(10, 5);cout << "Fecha: "<< factura->fecha.toString();

        gotoxy(10, 12);cout << "___________________________________________________________________";
        gotoxy(10, 14);cout << "Producto";
        gotoxy(30, 14);cout << "Cantidad";
        gotoxy(45, 14);cout << "Valor U.";
        gotoxy(60, 14);cout << "Total $";

        FacturaInfo *i = info_facturas( infos, factura->num);
        int y = 16;
        int total = 0;
        int totalProducto = 0;

        while ( i != NULL )
        {
            gotoxy(10, y); cout << i->producto;
            gotoxy(30, y); cout << i->cant;
            gotoxy(45, y); cout << i->valor_unitario;

            totalProducto 	= i->cant * i->valor_unitario;
            total 			+= totalProducto;

            gotoxy(60, y); cout << totalProducto;
            y ++;
            i = i->sig;
        }

        gotoxy(10, y+1);cout << "___________________________________________________________________";
        gotoxy(52, y+3);cout << "Total : " << total;

        switch( menu( 1+TECLA, 4+TECLA, 9) )
        {
            case 1 + TECLA:
            {
                string producto, nuevo_producto;
                int cant, valor_unitario;
                gotoxy(85, 2); cout << "Buscar Producto: ";
                producto = leer_letras();

                FacturaInfo *fi = buscar_producto( infos, factura->num ,producto );

                if ( fi == NULL )
                {
                    gotoxy(85, 5);cout << "Producto no encontrado";
                }

                gotoxy(85, 5); cout << "Editar producto ";

                gotoxy(85, 6); cout << "Producto: ";
                nuevo_producto = leer_letras();

                gotoxy(85, 7); cout << "Cantidad: ";
                cant    = atoi( leer_numeros().c_str() );

                gotoxy(85, 8); cout << "Precio unitario: ";
                valor_unitario = atoi( leer_numeros().c_str() );

                fi->producto = nuevo_producto;
                fi->cant     = cant;
                fi->valor_unitario = valor_unitario;


                gotoxy(85, 9); cout << "Producto editado con exito";
                getch();
                system("CLS");
            }
            break;

            case 2 + TECLA:
            {
                string day, month, year;
                gotoxy(85, 2); cout << "Nueva Fecha";
                gotoxy(85, 3);
                cout << "Dia: ";
                day = leer_numeros();
                gotoxy(85, 4);
                cout << "Mes: ";
                month = leer_numeros();
                gotoxy(85, 5);
                cout << "Anio: ";
                year = leer_numeros();

                Fecha nueva;
                nueva.datos( day, month, year);

                if ( nueva.valida() )
                {
                    factura->fecha = nueva;
                    gotoxy(85, 8); cout << "Fecha Editada con exito";
                }else
                {
                    gotoxy(85, 8); cout << "ERROR: Fecha no valida";
                }

                getch();
                system("CLS");
            }
            break;

            case 3 + TECLA:
            {
                string cliente;
                gotoxy(85, 2); cout << "Modificar cliente";
                gotoxy(85, 3);
                cout << "Cliente: ";
                cliente = leer_letras();
                factura->cliente = cliente;
                gotoxy(85, 8);cout << "Editado con exito";
                getch();
                system("CLS");
            }
            break;

            case 4 + TECLA:
            {
                salir = true;
                system("CLS");
            }
            break;
        }

    }while ( ! salir );
}


void informe_por_fecha()
{
    menus( 6 );
    string day;
    string month;
    string year;


    gotoxy(38, 9);  day     = leer_numeros();
    gotoxy(38, 10); month   = leer_numeros();
    gotoxy(38, 11); year    = leer_numeros();

    system("CLS");

    Fecha fecha;

    fecha.datos( day, month, year );

    Factura *fcts = facturas_por_fecha( facturas, fecha );

    gotoxy(10, 2);  cout << "Facturas del " << fecha.toString();
    gotoxy(10, 4);  cout << "___________________________________________________________________";
    gotoxy(10, 6);  cout << "#";
    gotoxy(20, 6);  cout << "Cliente";
    gotoxy(40, 6);  cout << "Total F.";

    int y = 8;
    int total_final_dia = 0;

    while( fcts )
    {
        gotoxy(10, y); cout << fcts->num;
        gotoxy(20, y); cout << fcts->cliente;

        int subtotal    = totalFactura( fcts->num );
        total_final_dia += subtotal;

        gotoxy(40, y); cout << subtotal;

        y++;
        fcts = fcts->sig;
    }

    gotoxy(10, y+1);cout << "___________________________________________________________________";
    gotoxy(32, y+3);cout << "Total : " << total_final_dia;

}


void informe_por_rango()
{
    string from_day, from_month, from_year, to_day, to_month, to_year;

    menus( 7 );

    gotoxy(38, 9); from_day     = leer_numeros();
    gotoxy(38, 10); from_month  = leer_numeros();
    gotoxy(39, 11); from_year   = leer_numeros();

    gotoxy(57, 9); to_day       = leer_numeros();
    gotoxy(57, 10); to_month    = leer_numeros();
    gotoxy(58, 11); to_year     = leer_numeros();


    system("CLS");

    Fecha from, to;

    from.datos( from_day, from_month, from_year );

    to.datos( to_day, to_month, to_year );

    gotoxy(10, 2); cout << "Facturas desde " << from.toString() << " Hasta " << to.toString();
    gotoxy(10, 4); cout << "___________________________________________________________________";
    gotoxy(10, 6); cout << "Fecha";
    gotoxy(30, 6); cout << "Total D.";

    int total_final_rango = 0;
    int total_dia = 0, y = 8;

    while( from.antes_de( to ) )
    {
        Factura *f_dias = facturas_por_fecha( facturas, from );

        while ( f_dias )
        {
            total_dia   += totalFactura( f_dias->num );
            f_dias      = f_dias->sig;
        }


        if ( total_dia != 0 )
        {
            gotoxy(10, y);cout << from.toString();
            gotoxy(30, y);cout << total_dia;

            total_final_rango += total_dia;
        }

        y ++;
        total_dia = 0;

        from.aumentar();
    }

    gotoxy(10, y+1);cout << "___________________________________________________________________";
    gotoxy(23, y+3);cout << "Total : " << total_final_rango;
}


Factura *insertar_factura( Factura *ptr, int num, Fecha fecha, string cliente )
{
    Factura *nueva  = new Factura();
    nueva->num      = num;
    nueva->fecha    = fecha;
    nueva->cliente  = cliente;

    nueva->sig      = NULL;

    if ( ptr == NULL )
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


Factura *facturas_por_fecha( Factura *ptr, Fecha fecha)
{
    Factura *lista  = NULL;
    Factura *i      = ptr;

    while ( i )
    {
        if ( i->fecha.igual( fecha ) )
        {
            lista = insertar_factura( lista, i->num, i->fecha, i->cliente );
        }

        i = i->sig;
    }

    return lista;
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

Factura *buscar_factura( Factura *ptr, int num )
{
    Factura *i = ptr;

    while( i )
    {
        if( i->num == num )
            return i;

        i = i->sig;
    }

    return i;
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
        ptr = nueva;
    } else
    {
        FacturaInfo *ant = ptr;
        while( ant->sig != NULL ) ant = ant->sig;
        ant->sig = nueva;
    }

    return ptr;
}

int totalFactura( int numFactura )
{
    FacturaInfo *info_f = info_facturas( infos, numFactura );
    int total = 0;

    while ( info_f )
    {
        total   += info_f->cant * info_f->valor_unitario;
        info_f  = info_f->sig;
    }

    return total;
}

bool existe_producto( FacturaInfo *ptr, string producto )
{
    FacturaInfo *i = ptr;

    while( i )
    {
        if( strcmpi( i->producto.c_str(), producto.c_str() ) == 0 )
            return true;
        i = i->sig;
    }


    return false;
}

FacturaInfo *buscar_producto( FacturaInfo *ptr, int numFactura,string producto)
{
    FacturaInfo *i = ptr;

    while( i )
    {
        if( i->numFactura == numFactura && strcmpi( i->producto.c_str(), producto.c_str() ) == 0 )
            return i;
        i = i->sig;
    }


    return i;
}

FacturaInfo *info_facturas( FacturaInfo *ptr, int numFactura )
{
    FacturaInfo *lista  = NULL;
    FacturaInfo * i     = ptr;

    while ( i )
    {
        if ( i->numFactura == numFactura )
            lista = insertar_facturaInfo( lista, numFactura, i->producto, i->cant, i->valor_unitario );
        i = i->sig;
    }
    return lista;
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
                    return tecla;
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
        gotoxy(30,5);cout << ("²          Sistema de facturas            ²");
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
        gotoxy(30,3);cout << ("²        Nuevo Producto          ²");
        gotoxy(30,4);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");
        gotoxy(30,5);cout << ("²            Producto            ²");
        gotoxy(30,6);cout << ("²                                ²");
        gotoxy(30,7);cout << ("²  Nombre:                       ²");
        gotoxy(30,8);cout << ("²  Cantidad:                     ²");
        gotoxy(30,9);cout << ("²  Valor unitario:               ²");
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

    if(tipo == 5)
    {
        gotoxy(30,2);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");
        gotoxy(30,3);cout << ("²        Buscar Factura          ²");
        gotoxy(30,4);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");
        gotoxy(30,5);cout << ("²           Factura              ²");
        gotoxy(30,6);cout << ("²                                ²");
        gotoxy(30,7);cout << ("²  Numero:                       ²");
        gotoxy(30,8);cout << ("²                                ²");
        gotoxy(30,9);cout << ("²                                ²");
        gotoxy(30,10);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");
    }

    if(tipo == 6)
    {
        gotoxy(30,2);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");
        gotoxy(30,3);cout << ("²        Facturas por Dia        ²");
        gotoxy(30,4);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");
        gotoxy(30,5);cout << ("²           Factura              ²");
        gotoxy(30,6);cout << ("²                                ²");
        gotoxy(30,7);cout << ("²  Fecha                         ²");
        gotoxy(30,8);cout << ("²                                ²");
        gotoxy(30,9);cout << ("²  Dia:                          ²");
        gotoxy(30,10);cout << ("²  Mes:                          ²");
        gotoxy(30,11);cout << ("²  Anio:                         ²");
        gotoxy(30,12);cout << ("²                                ²");
        gotoxy(30,13);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");
    }

    if(tipo == 7)
    {
        gotoxy(30,2);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");
        gotoxy(30,3);cout << ("²        Facturas por Dia        ²");
        gotoxy(30,4);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");
        gotoxy(30,5);cout << ("²           Factura              ²");
        gotoxy(30,6);cout << ("²                                ²");
        gotoxy(30,7);cout << ("²  Desde       |      Hasta      ²");
        gotoxy(30,8);cout << ("²              |                 ²");
        gotoxy(30,9);cout << ("²  Dia:        |      Dia:       ²");
        gotoxy(30,10);cout << ("²  Mes:        |      Mes:       ²");
        gotoxy(30,11);cout << ("²  Anio:       |      Anio:      ²");
        gotoxy(30,12);cout << ("²                                ²");
        gotoxy(30,13);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");
    }

    if(tipo == 8)
    {
        gotoxy(30,2);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");
        gotoxy(30,3);cout << ("²        Editar Factura          ²");
        gotoxy(30,4);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");
        gotoxy(30,5);cout << ("²           Factura              ²");
        gotoxy(30,6);cout << ("²                                ²");
        gotoxy(30,7);cout << ("²  Numero:                       ²");
        gotoxy(30,8);cout << ("²                                ²");
        gotoxy(30,9);cout << ("²                                ²");
        gotoxy(30,10);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");
    }


    if( tipo  == 9 )
    {
        gotoxy(50,2);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²");
        gotoxy(50,3);cout << ("² Menu                 ²");
        gotoxy(50,4);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²");
        gotoxy(50,5);cout << ("² 1-> Editar producto  ²");
        gotoxy(50,6);cout << ("² 2-> Editar Fecha     ²");
        gotoxy(50,7);cout << ("² 3-> Editar Cliente   ²");
        gotoxy(50,8);cout << ("² 4-> Salir            ²");
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
    que TAM pues de lo contrario, estarÃƒ­a accediendo
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
        decrementarÃ¯¿½ y p tendrÃ¯¿½a un subindice -1, "p[-1]"Ã¯¿½?*/
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
    que TAM pues de lo contrario, estarÃ¯¿½a accediendo
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
