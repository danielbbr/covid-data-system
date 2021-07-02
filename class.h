#ifndef CLASS_H
#define CLASS_H

#include <iostream>
#include <string>   
#include <fstream>
#include <vector>
#include <locale.h>
#include <iomanip>
#include <algorithm>
#include <ios>

using namespace std;

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

class Region;
class State;
class County;

class Info {
    public:
        string nomeRegiaoSaude, data;

        int tipo,
        codRegiaoSaude,
        semanaEpi,
        populacaoTCU2019, 
        casosAcumulado, 
        casosNovos, 
        obitosAcumulado, 
        obitosNovos, 
        Recuperadosnovos, 
        emAcompanhamentoNovos,
        interior_metropolitana;
    
        // Construtor para Region e State
        Info (int tipo, string data, int semanaEpi, int populacaoTCU2019, int casosAcumulado, int casosNovos, 
            int obitosAcumulado, int obitosNovos, int Recuperadosnovos, int emAcompanhamentoNovos) 
            : 
            tipo(tipo), data(data), semanaEpi(semanaEpi), populacaoTCU2019(populacaoTCU2019), casosAcumulado(casosAcumulado), 
            casosNovos(casosNovos), obitosAcumulado(obitosAcumulado), obitosNovos(obitosNovos), 
            Recuperadosnovos(Recuperadosnovos), emAcompanhamentoNovos(emAcompanhamentoNovos), 
            interior_metropolitana(-1) {
                nomeRegiaoSaude = "";
                codRegiaoSaude = 0;
            }

        // Construtor para County
        Info (int tipo, string nomeRegiaoSaude, string data, int codRegiaoSaude, int semanaEpi, int populacaoTCU2019, 
            int casosAcumulado, int casosNovos, int obitosAcumulado, int obitosNovos,
            int Recuperadosnovos, int emAcompanhamentoNovos, int interior_metropolitana) 
            : 
            tipo(tipo), nomeRegiaoSaude(nomeRegiaoSaude), data(data), codRegiaoSaude(codRegiaoSaude), semanaEpi(semanaEpi), 
            populacaoTCU2019(populacaoTCU2019), casosAcumulado(casosAcumulado), casosNovos(casosNovos),
            obitosAcumulado(obitosAcumulado), obitosNovos(obitosNovos), Recuperadosnovos(Recuperadosnovos), 
            emAcompanhamentoNovos(emAcompanhamentoNovos), interior_metropolitana(interior_metropolitana) {}

        // Construtor vazio
        Info (int tipo) 
            :
            tipo(tipo), nomeRegiaoSaude(""), data(""), codRegiaoSaude(0), semanaEpi(0), 
            populacaoTCU2019(0), casosAcumulado(0), casosNovos(0),
            obitosAcumulado(0), obitosNovos(0), Recuperadosnovos(0), 
            emAcompanhamentoNovos(0), interior_metropolitana(-1) {}
};

class Region { // Classe das Regiões
    private:
        string name;
    public:
        vector <State> state;
        vector <Info> info;
        string get_region_name() { return name; }
        Region(string name): name(name) {}
};

class State { // Classe dos Estados
    private:
        int uf;
        string name;
    public:
        vector <County> county;
        vector <Info> info;
        int get_state_uf() { return uf; }
        string get_state_name() { return name; }
        State (int uf, string name): uf(uf), name(name) {}
};

class County { // Classe dos Municípios
    private:
        string name;
        int codmun;
    public:
        vector <Info> info;
        string get_county_name() { return name; }
        County (string name, int codmun)
        : name(name), codmun(codmun) {}
};

#endif