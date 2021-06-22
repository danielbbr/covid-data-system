#include <iostream>
#include <string>   
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

class Info {
    private:
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
    public:
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

class Region {
    private:
        string name;
        Info info;
    public:
        string get_region_name() { return name; }
        Region(string name, Info info): name(name), info(info) {}
        Region(string name): name(name), info(0) {}
};

class State {
    private:
        Region region;
        int uf;
        string name;
        Info info;
    public:
        int get_state_uf() { return uf; }
        string get_state_name() { return name; }
        Region get_state_region() { return region; }
        State (int uf, string name, Region region, Info info): uf(uf), name(name), region(region), info(info) {}
        State (int uf, string name, Region region): uf(uf), name(name), region(region), info(1) {}
};

class County {
    private:
        State state;
        string name;
        int codmun;
        Info info;
    public:
        State get_county_state() { return state; }
        string get_county_name() { return name; }
        County (State state, string name, int codmun, Info info)
        : state(state), name(name), codmun(codmun), info(info) {}
};

// Classe que vai gerir todo o sistema
class System {
    private:
        vector <Region> region;
        vector <State> state;
        vector <County> county;
    public:
        System (string line){
            string region_aux, state_aux, county_aux;
            int coduf, codmun;
            region_aux = textUntilDivision(line,1);
            state_aux = textUntilDivision(line,1);
            county_aux = textUntilDivision(line,1);
            try{ coduf = stoi(textUntilDivision(line,1)); }catch(...){ coduf = 0; }
            try{ codmun = stoi(textUntilDivision(line,1)); }catch(...){ codmun = 0; }

            if (state_aux == "") // Adiciona região e sua Info no vector region
                region.push_back(Region(region_aux, lineToInfo(line,0)));
            else {
                
                // Acha a região correspondente
                int it_region;
                for(it_region=0;it_region<region.size();it_region++)
                    if(region[it_region].get_region_name() == Region(region_aux).get_region_name())
                        break;

                if(it_region == region.size()){
                    region.push_back(Region(region_aux, lineToInfo(line,0)));
                    it_region = region.size()-1;
                }

                if (county_aux == ""){
                    state.push_back(State(coduf, state_aux, region[it_region], lineToInfo(line,1)));
                }
                else {
                    // Acha o estado correspondente
                    int it_state;
                    for(it_state=0;it_state<state.size();it_state++)
                        if(state[it_state].get_state_name() == State(coduf, state_aux, region[it_region]).get_state_name())
                            break;

                    if(it_state == state.size()){
                        state.push_back(State(coduf, state_aux, region[it_region], lineToInfo(line,0)));
                        it_state = state.size()-1;
                    }

                    County(state[it_state], county_aux, codmun, lineToInfo(line,2));
                }
            }
        }

        // Método que retorna a string antes do ';' ou do '\n'
        string textUntilDivision (string& line, int deleteText){ 
            string text = "";
            int i=0;
            
            cout << "linha:" << line << endl;
            // Passa os valores até antes da divisão
            while (line[i]!=';' && line[i]!='\n' && line[i]!='\0'){
                text[i]=line[i];
                i++;
            }
            // Apaga a parte da linha até o ';' (incluso)
            if (deleteText){
                int aux;
                if(i == line.size()) // Não apagar +1 da memória se for o último elemento
                    aux=0;
                else
                    aux=1;
                for(int j=0;j<i+aux;j++)
                    line.erase (line.begin());
            }
                                
            return text;
        }

        Info lineToInfo (string line, int tipo){
            string nomeRegiaoSaude, data;

            int codRegiaoSaude,
            semanaEpi,
            populacaoTCU2019, 
            casosAcumulado, 
            casosNovos, 
            obitosAcumulado, 
            obitosNovos, 
            Recuperadosnovos, 
            emAcompanhamentoNovos,
            interior_metropolitana;

            try{ codRegiaoSaude = stoi(textUntilDivision(line,1)); }catch(...){ codRegiaoSaude = 0; }
            nomeRegiaoSaude = textUntilDivision(line,1);
            data = textUntilDivision(line,1);
            try{ semanaEpi = stoi(textUntilDivision(line,1)); }catch(...){ semanaEpi = 0; }
            try{ populacaoTCU2019 = stoi(textUntilDivision(line,1)); }catch(...){ populacaoTCU2019 = 0; }
            try{ casosAcumulado = stoi(textUntilDivision(line,1)); }catch(...){ casosAcumulado = 0; }
            try{ casosNovos = stoi(textUntilDivision(line,1)); }catch(...){ casosNovos = 0; }
            try{ obitosAcumulado = stoi(textUntilDivision(line,1)); }catch(...){ obitosAcumulado = 0; }
            try{ obitosNovos = stoi(textUntilDivision(line,1)); }catch(...){ obitosNovos = 0; }
            try{ Recuperadosnovos = stoi(textUntilDivision(line,1)); }catch(...){ Recuperadosnovos = 0; }
            try{ emAcompanhamentoNovos = stoi(textUntilDivision(line,1)); }catch(...){ emAcompanhamentoNovos = 0; }
            try{ interior_metropolitana = stoi(textUntilDivision(line,1)); }catch(...){ interior_metropolitana = -1; }
            
            return Info(tipo, nomeRegiaoSaude, data, codRegiaoSaude, semanaEpi, populacaoTCU2019, casosAcumulado, 
            casosNovos, obitosAcumulado, obitosNovos, Recuperadosnovos, emAcompanhamentoNovos, interior_metropolitana);
        }
};

int main() {

    string text;

    ifstream File("HIST_PAINEL_COVIDBR_Parte3_20jun2021.csv");

    getline (File, text);
    cout << text <<endl;

    getline (File, text);
    cout << text <<endl;

    System t = text;
    t.textUntilDivision(text,1);
    cout << text << endl;

    getline (File, text);
    cout << text <<endl;

    File.close();
}