#ifndef CLASS_H
#define CLASS_H

#include <iostream>
#include <string>   
#include <fstream>
#include <vector>
#include <locale.h>
#include <iomanip>
 
using namespace std;

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