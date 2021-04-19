/*
REQUISITOS
La empresa "Credi-GDL" necesita un sistema para llevar el 
control de las cuentas de sus clientes.

-> Domínguez Martínez Edder Emmanuel
-> Esqueda Rodríguez, Fabián Alejandro.
-> Godoy Rodríguez, Berenice.
-> Gómez Jiménez, Ángel Armando.
-> Gómez Ramírez, Eduardo Fabián.

Sección: D05
Calendario: 2021-A
*/


#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include "cliente.h"

using namespace std;

//Prototipos de las funciones para el menu
void alta_de_clientes(fstream &);//Recibe el archivo logico como parametro
void eliminar_registro(fstream &);//y es una referencia, porque va a haber cambios
void actualizar_registro(fstream &);
void consultas_individuales(fstream &);
void consulta_general(fstream &);

int main(){
    system("color F4");
    system("cls");
    
    //Menu de opciones
    size_t opc;

    do{
        cout<<"\t\t\t\t\tMenu - Sistema de Cuentas"<<endl<<endl;
        cout<<"1. ALTAS"<<endl;
        cout<<"2. BAJAS (ELIMINAR UN REGISTRO)"<<endl;
        cout<<"3. CAMBIOS (ACTUALIZAR EL SALDO DE LA CUENTA"<<endl;
        cout<<"4. CONSULTAS INDIVIDUALES (NUMERO DE CUENTA)"<<endl;
        cout<<"5. CONSULTAS GENERALES"<<endl;
        cout<<"6. SALIR"<<endl<<endl;

        cout<<"ELIGE UNA OPCION: ";
        cin>>opc;
        cin.ignore();

        //Creando el archivo logico
        fstream archivoCreditoEntradaSalida("credito.dat", ios::in | ios::out);
    
        if(!archivoCreditoEntradaSalida){
            cerr<<"\n\n\n\t\t\tNO DE PUDO ABRIR EL ARCHIVO DE MANERA CORRECTA\n\n\n";
            exit(1);
        }

        system("cls");

        //Switch para las opciones del menu
        switch(opc){
            case 1:  //Opcion para dar de alta clientes
                    alta_de_clientes(archivoCreditoEntradaSalida);//Se le manda el archivo lógico
                    cout<<"\n\n\n"; system("pause"); system("cls");
                    break; 

            case 2: //Opcion para dar de baja registros
                    eliminar_registro(archivoCreditoEntradaSalida);
                    cout<<"\n\n\n"; system("pause"); system("cls");
                    break;
            
            case 3: //Opcion para actualizar el saldo de la cuenta
                    actualizar_registro(archivoCreditoEntradaSalida);
                    cout<<"\n\n\n"; system("pause"); system("cls");
                    break;

            case 4: //Opcion para consultar un registro individual
                    consultas_individuales(archivoCreditoEntradaSalida);
                    cout<<"\n\n\n"; system("pause"); system("cls");
                    break;

            case 5: //opcion para consultar todos los registros
                    consulta_general(archivoCreditoEntradaSalida);
                    cout<<"\n\n\n"; system("pause"); system("cls");
                    break;
        }
       
        archivoCreditoEntradaSalida.clear();
        archivoCreditoEntradaSalida.close();

    }while(opc != 6);//opcion para salir

    return 0;
}

//Funcion para dar de alta clientes
void alta_de_clientes(fstream &agregarCliente){
    cout<<"\t\t\t\tDAR DE ALTA UN CLIENTE"<<endl<<endl;

    //Atributos que estan en la clase
    int numeroCuenta;
    char apellido[15];
    char primerNombre[10];
    double saldo;

    Cliente cliente;//Objeto de tipo cliente
    size_t i = 0;

    cout<<"Escriba el numero de cuenta (de 1 a 100, 0 para terminar la entrada)\n? ";
    cin>>numeroCuenta;

    // desplaza el apuntador de posición del archivo al registro correcto en el archivo
    agregarCliente.seekg((numeroCuenta - 1) * sizeof(Cliente));
    //Leer un registro
    agregarCliente.read(reinterpret_cast<char * >(&cliente), sizeof(Cliente));

    //Validar si el numero de cuenta no existe
    if(cliente.obtenerNumeroCuenta() == 0){//Crea un registro, si no es que ya existe
        while(numeroCuenta > 0 && numeroCuenta <= 100){
            cout<<"Ingrese su apellido: ";
            cin>>setw(15)>>apellido;//En el setw tiene 15 porque ese es el tamaño del arreglo apellido

            cout<<"Ingrese su primer nombre: ";
            cin>>setw(10)>>primerNombre;//En el setw tiene 10 porque ese es el tamaño del arreglo primerNombre

            cout<<"Ingrese su saldo: ";
            cin>>saldo;

            //Establecer los valores de los atributos
            cliente.establecerNumeroCuenta(numeroCuenta);
            cliente.establecerApellido(apellido);
            cliente.establecerPrimerNombre(primerNombre);
            cliente.establecerSaldo(saldo);

            //busca la posición en el archivo del registro especificado por el usuario
            //Busca en que posicion del archivo voy a escribir cliente
            agregarCliente.seekp((cliente.obtenerNumeroCuenta()-1) * sizeof(Cliente));

            //escribe la información especificada por el usuario en el archivo
            agregarCliente.write(reinterpret_cast<const char *>(&cliente), sizeof(Cliente));

            //Hacer otro registro
            cout<<"\nEscriba el numero de cuenta (de 1 a 100, 0 para terminar la entrada)\n?";
            cin>>numeroCuenta;

        }//Fin del while()
    }//Fin del if

    else{//Si el numero de cuenta ya existe
        cout<<"\n\n\n\t\t\tEL NUMERO DE CUENTA -> "<<numeroCuenta<<" <- YA EXISTE EN LOS REGISTROS"<<endl<<endl;
    }

}//Fin de la funcion

//Eliminar un registro
void eliminar_registro(fstream &eliminarCliente){
    cout<<"\t\t\t\tDAR DE BAJA UN CLIENTE"<<endl<<endl;

    int numeroCuenta;
    char op;
    Cliente cliente;

    cout<<"\nDigita el numero de cuenta que deseas eliminar: ";
    cin>>numeroCuenta;

    //Se desplaza el apuntador hacia el registro a eliminar
    eliminarCliente.seekg((numeroCuenta-1) * sizeof(Cliente));

    //Leer un registro
    eliminarCliente.read(reinterpret_cast<char *> (&cliente), sizeof(Cliente));

    if(cliente.obtenerNumeroCuenta() != 0){
        Cliente clienteBlanco;

        cout<<"\n\nEstas seguro de querer eliminar este registro (s/n)? ";
        cin>>op;

        if(op == 's' || op == 'S'){
            eliminarCliente.seekp((numeroCuenta-1)*sizeof(Cliente));
            eliminarCliente.write(reinterpret_cast<char *>(&clienteBlanco), sizeof(Cliente));

            cout<<"\n\n\t\tEL REGISTRO CON EL NUMERO DE CUENTA -> "<<numeroCuenta<<" <- HA SIDO ELIMINADO"<<endl<<endl;
        }
        else{
            cout<<"\n\n\t\t\tLA ACCION HA SIDO CANCELADA\n\n\n";
        }
    }
}

//Actualizar el saldo de la cuenta
void actualizar_registro(fstream &actualizarCliente){
    cout<<"\t\t\t\tACTUALIZAR SALDO DE UN CLIENTE"<<endl<<endl;

    int numeroCuenta;
    double transaccion, saldoAnterior;
    Cliente clienteActualizar;

    cout<<"Digita el numero de cuenta que quieres actualizar: ";
    cin>>numeroCuenta;

    //Desplazar el apuntador hasta el registro correcto
    actualizarCliente.seekg((numeroCuenta-1)*sizeof(Cliente));

    //Leer el registro
    actualizarCliente.read(reinterpret_cast<char *>(&clienteActualizar), sizeof(Cliente));

    cout<<endl;

    cout<<left<<setw(10)<<"CUENTA"<<setw(16)<<"APELLIDO"<<setw(11)<<"NOMBRE"<<left<<
    setw(10)<<right<<"SALDO"<<endl;

    //actualizar el registro, es decir, actualizar el saldo únicamente
    if(clienteActualizar.obtenerNumeroCuenta() != 0){
        //Mostrando el registro
        cout<<left<<setw(10)<<clienteActualizar.obtenerNumeroCuenta()
            <<setw(16)<<clienteActualizar.obtenerApellido()
            <<setw(11)<<clienteActualizar.obtenerPrimerNombre()
            <<setw(10)<<setprecision(2)<<right<<fixed 
            <<showpoint<<clienteActualizar.obtenerSaldo()<<endl;

            //Pedirle al usuario que tipo de transaccion quiere hacer
            cout<<"\nEscriba cargo (+) o abono (-): ";
            cin>>transaccion;

            //Actualizar el saldo del registro
            saldoAnterior = clienteActualizar.obtenerSaldo();//Se guarda el saldo actual del cliente
            clienteActualizar.establecerSaldo(saldoAnterior + transaccion);//El nuevo saldo es la suma de 
                                                                            //la transaccion mas el saldo anterior

    
        cout<<left<<setw(10)<<"CUENTA"<<setw(16)<<"APELLIDO"<<setw(11)<<"NOMBRE"<<left<<
        setw(10)<<right<<"SALDO"<<endl;

        //Mostrar registro
        cout<<left<<setw(10)<<clienteActualizar.obtenerNumeroCuenta()
            <<setw(16)<<clienteActualizar.obtenerApellido()
            <<setw(11)<<clienteActualizar.obtenerPrimerNombre()
            <<setw(10)<<setprecision(2)<<right<<fixed 
            <<showpoint<<clienteActualizar.obtenerSaldo()<<endl;

        //desplazar el apuntador hacia el registro corregcto
        actualizarCliente.seekp((numeroCuenta-1)*sizeof(Cliente));
        //Escribir el registro actualizado
        actualizarCliente.write(reinterpret_cast<const char *>(&clienteActualizar), sizeof(Cliente));
    }

    else{
        cerr<<"\n\n\t\tEL NUMERO DE CUENTA -> "<<numeroCuenta<<" <- NO TIENE INFORMACION O NO EXISTE"<<endl<<endl;
    }
}

//Consultas individuales
void consultas_individuales(fstream &consultaIndividual){
    cout<<"\t\t\t\tCONSULTA INDIVIDUAL (NUMERO DE CUENTA)\n\n";

    int numeroCuenta;
    Cliente consultaCliente;

    cout<<"Teclea el numero de cuenta del cliente: ";
    cin>>numeroCuenta;

    consultaCliente.establecerNumeroCuenta(numeroCuenta);

    //buscar en la posición en el archivo del registro especificado por el usuario
    consultaIndividual.seekg((consultaCliente.obtenerNumeroCuenta()-1) * sizeof(Cliente));//seekg(n, ios::beg)

    //Leer la información especificada por el usuario en el archivo
    consultaIndividual.read(reinterpret_cast <char *>(&consultaCliente), sizeof(Cliente));
    
    if(numeroCuenta != consultaCliente.obtenerNumeroCuenta()){
        cout<<"\n\n\n\t\t\tEL NUMERO DE CUENTA NO EXISTE...\n\n\n";
    }

    else{
        cout<<endl;
        cout<<"CUENTA       : "<<consultaCliente.obtenerNumeroCuenta()<<endl;
        cout<<"APELLIDO     : "<<consultaCliente.obtenerApellido()<<endl;
        cout<<"PRIMER NOMBRE: "<<consultaCliente.obtenerPrimerNombre()<<endl;
        cout<<"SALDO        : $ "<<consultaCliente.obtenerSaldo()<<endl;
    }
}

//Consulta general
void consulta_general(fstream &consultaGeneral){
    cout<<"\t\t\t\tCONSULTA GENERAL\n\n";
    Cliente clientes;

    //Esta linea me esta leyendo el primero registro del archivo
    consultaGeneral.read(reinterpret_cast<char *> (&clientes), sizeof(Cliente));
    
    cout<<left<<setw(10)<<"CUENTA"<<setw(16)<<"APELLIDO"<<setw(11)<<"NOMBRE"<<left<<
    setw(10)<<right<<"SALDO"<<endl;

    //Mientras el archvivo se haya abierto (esté bien) y no sea el final del archivo
    while(consultaGeneral && !consultaGeneral.eof()){ 
        //Imprimir el primer registro
        if(clientes.obtenerNumeroCuenta() != 0){
            cout<<left<<setw(10)<<clientes.obtenerNumeroCuenta()
            << setw(16)<<clientes.obtenerApellido()
            << setw(11)<<clientes.obtenerPrimerNombre()<<left
            << setw(10)<<setprecision(2)<<right<<fixed<<showpoint<<clientes.obtenerSaldo()<<endl;
        }
        //Leyendo el siguiente registro en el archivo
        consultaGeneral.read(reinterpret_cast<char *>(&clientes), sizeof(Cliente));
    }//Fin del while()
}