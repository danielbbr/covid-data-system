#include "class.h"

using namespace std;

class System { // Classe que vai gerir todo o sistema
    protected:
        bool isEnd = false;
        int esc_region, esc_state, esc_county;
        vector <Info> *ptr;
    public:
        vector <Region> region;
        // Método que retorna a string antes do ';' ou do '\n'
        string textUntilDivision (string& line, int deleteText){ 
            string text = "";
            int i=0;
            
            // Passa os valores até antes da divisão
            while (line[i]!=';' && line[i]!='\n' && line[i]!='\0' && line[i]!='/'){
                text+=line[i];
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
        
        // Passa uma string (linha do csv) para Info (classe dos dados)
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

        void Allocate(ifstream &File){ // Método que lê todas as informações do .csv no programa
            int cont=0;
            do{
                cont++;
                if (!(cont%10000)){ // Atualiza a tela com uma porcentagem do carregamento
                    //system("CLS");
                    cout << "Por favor, aguarde. Estamos atualizando nossos dados..." << endl;
                    cout << "Carregamento: " << fixed << setprecision(2) << ( ((float) cont/960000)*100 ) << "%" << endl << endl;
                }
                string line;
                getline (File,line);
                string region_aux, state_aux, county_aux;
                int coduf, codmun;

                region_aux = textUntilDivision(line,1);
                state_aux = textUntilDivision(line,1);
                county_aux = textUntilDivision(line,1);
                try{ coduf = stoi(textUntilDivision(line,1)); }catch(...){ coduf = 0; }
                try{ codmun = stoi(textUntilDivision(line,1)); }catch(...){ codmun = 0; }

                if (!File.eof()) { // Somente roda se ainda existir algo para ser lido no arquivo

                    // Acha a região correspondente
                    int it_region=0;
                    for(it_region=0;it_region<region.size();it_region++)
                        if(region[it_region].get_region_name() == region_aux)
                            break;

                    if(it_region == region.size()){
                        region.push_back(Region(region_aux));
                        it_region = region.size()-1;
    //                    cout << "Create Region:" << region_aux << endl;
                    }

                    if (state_aux == "") // Adiciona região e sua Info no vector region
                        (region[it_region].info).push_back(lineToInfo(line,0));
                    else {
                        // Acha o estado correspondente
                        int it_state=0;
                        for (it_state=0;it_state<(region[it_region].state).size();it_state++)
                            if ((region[it_region].state)[it_state].get_state_name() == state_aux)
                                break;

                        if (it_state == (region[it_region].state).size()){
                            (region[it_region].state).push_back(State(coduf, state_aux));
                            it_state = (region[it_region].state).size()-1;
    //                        cout << "Create State:" << state_aux << endl;
                        }

                        if (county_aux == "")
                            ((region[it_region].state[it_state]).info).push_back(lineToInfo(line,1));
                        else {
                            // Acha o município correspondente
                            int it_county=0;
                            for (it_county=0;it_county<((region[it_region].state[it_state]).county).size();it_county++)
                                if (((region[it_region].state[it_state]).county[it_county]).get_county_name() == county_aux)
                                    break;

                        //    cout << "Is County:" << county_aux << endl;
                            if (it_county == ((region[it_region].state[it_state]).county).size()){
    //                            cout << "Create County:" << county_aux << endl;
                                ((region[it_region].state[it_state]).county).push_back(County(county_aux, codmun));
                                it_county = ((region[it_region].state[it_state]).county).size()-1;
                            }

                            (((region[it_region].state[it_state]).county[it_county]).info).push_back(lineToInfo(line,2));
                        }
                    }
                }
            } while (!File.eof());
            //system("CLS");
            cout << "Dados carregados com sucesso!" << endl << endl;
        }

        void escRegion(){ // Escolhe as regiões a partir da entrada do usuário
            string esc_aux;
            bool repeat=false;
            do {
                esc_region=-1;
                //system("CLS");
                cout << "[ SISTEMA DE ACOMPANHAMENTO DO COVID NO BRASIL ]" << endl;
                if (repeat)
                    cout << "Escolha uma opcao valida!" << endl;
                cout << "Qual regiao deseja analisar?" << endl;
                    
                for(int i=0;i<region.size();i++)
                    cout << i << " - " << region[i].get_region_name() << endl;

                cout << region.size() << " - Sair do programa" << endl;

                cout << "Escolha: ";
                cin >> esc_aux; // Escolha do usuário
                try {
                    esc_region = stoi(esc_aux);

                    if(esc_region==region.size()){
                        isEnd = true;
                        break;
                    } else 
                        if (esc_region<0 || esc_region>region.size())
                            repeat = true;
                        else {
                            escState(esc_region); // A depender da escolho do usuário, entra em um estado específico
                            repeat = false;
                        }
                } catch(...) {
                    repeat = true;
                }
            } while (!(isEnd));
        }

        void escState(int esc_region){ // Escolhe os estados a partir da entrada do usuário
            esc_state = -1;
            string esc_state_aux;
            bool repeat=false;
            do {
                //system("CLS");
                cout << "[ SISTEMA DE ACOMPANHAMENTO DO COVID NO BRASIL ]" << endl;
                cout << "[ REGIAO " << region[esc_region].get_region_name() << " ]" << endl;
                if (repeat)
                    cout << "Escolha uma opcao valida!" << endl;
                cout << "Qual estado deseja analisar?" << endl;
                
                for(int i=0;i<(region[esc_region].state).size();i++)
                    cout << i << " - " << (region[esc_region].state[i]).get_state_name() << endl;
                
                cout << (region[esc_region].state).size() << " - Analisar a propria regiao " << region[esc_region].get_region_name() << endl;
                cout << (region[esc_region].state).size()+1 << " - Voltar para o menu inicial" << endl;
                cout << (region[esc_region].state).size()+2 << " - Sair do programa" << endl;

                cout << "Escolha: ";
                cin >> esc_state_aux; // Escolha do usuário
                try {
                    esc_state = stoi(esc_state_aux);
                    
                    if(esc_state==(region[esc_region].state).size()+1)
                        break;
                    if(esc_state==(region[esc_region].state).size()+2){
                        isEnd = true;
                        break;
                    }
                    else 
                        if (esc_state<0 || esc_state>(region[esc_region].state).size()+2)
                            repeat = true;
                        else{
                            if (esc_state==(region[esc_region].state).size())
                                funcInfos(0); // Analisa os dados da própria região [A SER IMPLEMENTADO]
                            else 
                                escCounty(); // Analisa os municipios de um determinado estado 
                            break;
                            repeat = false;
                        }
                } catch(...) {
                    repeat = true;
                }
            } while (1);
        }
        
        void escCounty(){ // Escolhe os municipios a partir da entrada do usuário
            esc_county = -1;
            string esc_county_aux;
            bool repeat=false;
            do {
                //system("CLS");
                cout << "[ SISTEMA DE ACOMPANHAMENTO DO COVID NO BRASIL ]" << endl;
                cout << "[ REGIAO " << region[esc_region].get_region_name() << " ]" << endl;
                cout << "[ ESTADO " << (region[esc_region].state[esc_state]).get_state_name() << " ]" << endl;

                if (repeat)
                    cout << endl << "Escolha uma opcao valida!" << endl;
                cout << "Qual municipio deseja analisar?" << endl;
                
                for(int i=0;i<((region[esc_region].state[esc_state]).county).size();i++)
                    cout << i << " - " << ((region[esc_region].state[esc_state]).county[i]).get_county_name() << endl;
                
                cout << ((region[esc_region].state[esc_state]).county).size() << " - Analisar o proprio estado " << (region[esc_region].state[esc_state]).get_state_name() << endl;
                cout << ((region[esc_region].state[esc_state]).county).size()+1 << " - Voltar para o menu inicial" << endl;
                cout << ((region[esc_region].state[esc_state]).county).size()+2 << " - Sair do programa" << endl;
                
                cout << "Escolha: ";
                cin >> esc_county_aux; // Escolha do usuario
                try {
                    esc_county = stoi(esc_county_aux);

                    if(esc_county==((region[esc_region].state[esc_state]).county).size()+1)
                        break;
                    if(esc_county==((region[esc_region].state[esc_state]).county).size()+2){
                        isEnd = true;
                        break;
                    }
                    else{
                        if (esc_county<0 || esc_county>((region[esc_region].state[esc_state]).county).size()+2)
                            repeat = true;
                        else{
                            if(esc_county==((region[esc_region].state[esc_state]).county).size()) // funcInfos() para o State
                                funcInfos(1); // Analisa os dados do próprio estado [A SER IMPLEMENTADO]
                            else { // funcInfos() para os County
                                funcInfos(2); // Analisa os dados de um dado municipio [A SER IMPLEMENTADO]
                            }
                            break;
                            repeat = false;
                        }
                    }
                } catch(...) {
                    repeat = true;
                }
            } while (1);
        }
        
        void histogram() {  //cidade/regiao, uma data inicial e uma data final
    
    /*
    for(int i = largest; i >= 1; i--)

    Inside the body of the loop, do steps 3 to 5 inclusive

    If i <= value_of_column_a then print a *, otherwise print a space

    Repeat step 3 for each column (or write a loop...)

    Print a newline character

    Print the horizontal line using -

    Print the column labels
    */
    }
    
        
        
        // Realiza todas as manipulações com os dados que o usuário quiser
        void funcInfos(int tipo){
            cout << "I'm in funcInfos " << tipo << " !" << endl;
            
            //system("CLS");
            cout << "[ SISTEMA DE ACOMPANHAMENTO DO COVID NO BRASIL ]" << endl;
            cout << "[ REGIAO " << region[esc_region].get_region_name() << " ]" << endl;
            
            // Passa para o ponteiro o vector <Info> em cada caso
            if (tipo==0)
                ptr = &(region[esc_region].info);
            else {
                cout << "[ ESTADO " << (region[esc_region].state[esc_state]).get_state_name() << " ]" << endl;
                if (tipo==1)
                    ptr = &((region[esc_region].state[esc_state]).info);
                else {
                    ptr = &(((region[esc_region].state[esc_state]).county[esc_county]).info);
                    cout << "[ MUNICIPIO " << ((region[esc_region].state[esc_state]).county[esc_county]).get_county_name() << " ]" << endl;
                }
            }

            bool repeat = false;
            bool endDo = false;
            string esc_aux;
            int esc;
            do { // Menu que recebe as escolhas para dado local
                //system("CLS");
                cout << "[ SISTEMA DE ACOMPANHAMENTO DO COVID NO BRASIL ]" << endl;
                cout << "[ REGIAO " << region[esc_region].get_region_name() << " ]" << endl;
                if (tipo>0) {
                    cout << "[ ESTADO " << (region[esc_region].state[esc_state]).get_state_name() << " ]" << endl;
                    if (tipo==1)
                        ptr = &((region[esc_region].state[esc_state]).info);
                    else 
                        cout << "[ MUNICIPIO " << ((region[esc_region].state[esc_state]).county[esc_county]).get_county_name() << " ]" << endl;
                }

                if (repeat)
                    cout << endl << "Escolha uma opcao valida!" << endl;
                cout << "0 - Total de casos em determinada data" << endl;
                cout << "1 - Total de obitos em determinada data" << endl;
                cout << "2 - Calculo da media movel de casos em determinada data" << endl;
                cout << "3 - Calculo da media movel de obitos em determinada data" << endl;
                cout << "4 - Calculo da tendencia de crescimento de casos entre duas medias moveis" << endl;
                cout << "5 - Calculo da tendencia de crescimento de obitos entre duas medias moveis" << endl;
                cout << "6 - Plotar histograma" << endl;
                cout << "7 - Voltar para o menu inicial" << endl;
                cout << "8 - Sair do programa" << endl;

                cout << "Escolha: ";
                cin >> esc_aux; // Escolha do usuario
                try {
                    esc = stoi(esc_aux);
                    string data1="", data2="";
                    int aux=0;
                    float auxf=0.0;
                    switch (esc){
                        case 0: // Total de casos em determinada data
                            aux = totalDeCasosObitos(esc);
                            cout << "Total de novos casos: " << aux << endl;
                            break;
                        case 1: // Total de obitos em determinada data
                            aux = totalDeCasosObitos(esc);
                            cout << "Total de novos obitos: " << aux << endl;
                            break;
                        case 2: // Cálculo da média móvel de casos em determinada data
                            auxf = mediaMovel(false,0,"");
                            cout << "Media movel de casos: " << auxf << endl;
                            break;
                        case 3: // Cálculo da média móvel de obitos em determinada data
                            auxf = mediaMovel(true,0,"");
                            cout << "Media movel de obitos: " << auxf << endl;
                            break;
                        case 4: // Cálculo da tendência de crescimento de casos entre duas médias móveis
                            aux = tendenciaCresc(false);
                            if (aux==1)
                                cout << "Situacao em crescimento" << endl;
                            else
                                if (aux==-1)
                                    cout << "Situacao em queda" << endl;
                                else
                                    cout << "Situacao estavel" << endl;
                            break;
                        case 5: // Cálculo da tendência de crescimento de obitos entre duas médias móveis
                            aux = tendenciaCresc(true);
                            if (aux==1)
                                cout << "Situacao em crescimento" << endl;
                            else
                                if (aux==-1)
                                    cout << "Situacao em queda" << endl;
                                else
                                    cout << "Situacao estavel" << endl;
                            break;
                        case 6: // [A ESCOLHER UMA PERGUNTA ÚNICA]
                            break;
                        case 7: // Voltar para o menu inicial
                            endDo = true;
                            break;
                        case 8: // Sair do programa
                            isEnd = true;
                            endDo = true;
                            break;
                        default:
                            repeat = true;
                    }
                    programPause();
                    if (esc>=0 && esc<=8)
                        repeat = false;
                } catch(...) {
                    repeat = true;
                }
            } while (!(endDo));
        }

        string lerData (){
            string data, aux="";
            bool repeat = false;
            int dia, mes, ano;
            bool isDataOk = false;

            do {
                if (repeat)
                    cout << "\nInsira uma data valida!" << endl;
                cout << "Entre com a data desejada no formato (DD/MM/AAAA): ";
                cin >> data;
                aux = textUntilDivision(data,1);
//                cout << "dia lido:" << aux << endl;
                try {
                    dia = stoi(aux);
                    aux = textUntilDivision(data,1);
//                    cout << "mes lido:" << aux << endl;
                    try {
                        mes = stoi(aux);
                        aux = textUntilDivision(data,1);
//                        cout << "ano lido:" << aux << endl;
                        try {
                            ano = stoi(aux);
                            if(dia<1 || dia>31 || mes<1 || mes>12 || ano<2000 || ano>2021) 
                                repeat = true;
                            else {
                                data="";
                                data += to_string(ano)+"-";
                                if (mes<10)
                                    data += "0";
                                data += to_string(mes)+"-";
                                if (dia<10)
                                    data += "0";
                                data += to_string(dia);

                            /*    cout << "Data final:" << data << endl;

                                cout << "Data begin:" << (ptr->begin())->data << endl;
                                cout << "Data end:" << (--(ptr->end()))->data << endl;*/

                                if ((ptr->begin())->data<=data && (--(ptr->end()))->data>=data){
                                    repeat = false;
                                    isDataOk = true;
                                }
                                else
                                    repeat = true;
                            }
                        } catch (...) {
                            repeat = true;
                        }
                    } catch (...) {
                        repeat = true;
                    }
                } catch (...) {
                    repeat = true;
                }
            } while (!(isDataOk));
//            programPause();
            return data;
        }

        int totalDeCasosObitos (int esc) {
            string data1, data2;
            int cont=0;
            do {
                if (cont)
                    cout << "A data inicial nao pode ser maior que a data final!" << endl;
                cout << "Insira as datas inicial e final, respectivamente, para a analise." << endl;
                data1=""; data2="";
                data1 = lerData();
//                cout << "1a Data lida: " << data1 << endl;
                data2 = lerData();
//                cout << "2a Data lida: " << data2 << endl;
                cont++;
            } while (data1 > data2);

            auto it = ptr->begin();

            while (it->data!=data1) {
                it++;
            };

            int total=0;
            if (esc==0)
                total = it->casosNovos;
            else
                total = it->obitosNovos;

            while (it->data!=data2) {
                it++;
                if (esc==0)
                    total += it->casosNovos;
                else
                    total += it->obitosNovos;
            };

            return total;
        }

        float mediaMovel (bool casosObitos, int janela, string data) {
            float media=0;
            string sjanela;
            bool repeat;
            if (data=="")
                data = lerData();
            repeat = false;
            auto it = ptr->begin();
            do {
                if (janela==0) {
                    if (repeat)
                        cout << "Janela para media movel invalida!" << endl;
                    cout << "Insira a janela para o calculo da media movel: ";
                    cin >> sjanela;
                }
                try {
                    if (janela==0) 
                        janela = stoi (sjanela);
                    if (janela>0){
                        it = ptr->begin();
                        while (it->data!=data) {
                            it++;
                        };

                        if (distance(ptr->begin(), it)>=janela-1)
                            break;
                        else
                            repeat = true;
                    } else
                        repeat = true;
                } catch (...) {
                    repeat = true;
                }
            } while (1);

            int i=janela-1;
            do {
                if (casosObitos)
                    media += it->obitosNovos;
                else
                    media += it->casosNovos;
                it--;
                i--;
            } while (i>=0);

            media = media / (janela);

            return media;
        }

        int tendenciaCresc (bool casosObitos) {
            float media, media_ant;
            bool repeat = false;
            string sjanela, sintervalo;
            int janela, intervalo;
            auto it = ptr->begin();
            auto it_ant = ptr->begin();
            string data, data_ant;
            do {
                data = lerData();
                do {
                    if (repeat)
                        cout << "Janela para media movel invalida!" << endl;
                    cout << "Insira a janela para o calculo da media movel: ";
                    cin >> sjanela;
                    try {
                        janela = stoi (sjanela);
                        if (janela>0){
                            it = ptr->begin();
                            while (it->data!=data) {
                                it++;
                            };

                            if (distance(ptr->begin(), it)>=janela-1) {
                                repeat = false;
                                break;
                            } else
                                repeat = true;
                        } else
                            repeat = true;
                    } catch (...) {
                        repeat = true;
                    }
                } while (repeat);

                repeat = false;
                do {
                    if (repeat)
                        cout << "Intervalo invalido!" << endl;
                    cout << "Insira o intervalo para o calculo da tendencia de crescimento: ";
                    cin >> sintervalo;
                    try {
                        intervalo = stoi (sintervalo);
                        if (intervalo>0 && distance(ptr->begin(), it)>=janela+intervalo-1){
                            it_ant = it;
                            int cont = 0;
                            while (cont<intervalo) {
                                it_ant--;
                                cont++;
                            };
                            data_ant = it_ant->data;
                            repeat = false;
                            break;
                        } else
                            repeat = true;
                    } catch (...) {
                        repeat = true;
                    }
                } while (repeat);
            } while (repeat);

            media = mediaMovel(casosObitos,janela,data);
            media_ant = mediaMovel(casosObitos,janela,data_ant);

            if (media/media_ant >= 1.15)
                return 1;
            else
                if (media/media_ant <= 0.85)
                    return -1;
                else
                    return 0;
        }
        void programPause(){
	        cout << "Pressione enter para continuar...";
	        getchar();
	        getchar();
	        cout << endl;
}
        System (ifstream &File){
            Allocate(File);
            escRegion();
        }
};

int main() {
    string text;

    //setlocale(LC_ALL, "Portuguese");

    ifstream File("HIST_PAINEL_COVIDBR_Parte3_20jun2021.csv");

    getline (File, text);

    System t = System(File);

    cout << "Programa finalizado com sucesso!" << endl;

    File.close();
}