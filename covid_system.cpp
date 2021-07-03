#include "class.h"

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
                        }

                        if (county_aux == "")
                            ((region[it_region].state[it_state]).info).push_back(lineToInfo(line,1));
                        else {
                            // Acha o município correspondente
                            int it_county=0;
                            for (it_county=0;it_county<((region[it_region].state[it_state]).county).size();it_county++)
                                if (((region[it_region].state[it_state]).county[it_county]).get_county_name() == county_aux)
                                    break;

                            if (it_county == ((region[it_region].state[it_state]).county).size()){
                                ((region[it_region].state[it_state]).county).push_back(County(county_aux, codmun));
                                it_county = ((region[it_region].state[it_state]).county).size()-1;
                            }

                            (((region[it_region].state[it_state]).county[it_county]).info).push_back(lineToInfo(line,2));
                        }
                    }
                }
            } while (!File.eof());
            cout << "Dados carregados com sucesso!\n" << endl;
        }

        void escRegion(){ // Escolhe as regiões a partir da entrada do usuário
            string esc_aux;
            bool repeat=false;
            do {
                esc_region=-1;
                cout << "\n[ SISTEMA DE ACOMPANHAMENTO DO COVID NO BRASIL ]\n" << endl;
                if (repeat)
                    cout << "Escolha uma opcao valida!\n" << endl;
                cout << "Qual regiao deseja analisar?" << endl;
                    
                for(int i=0;i<region.size();i++)
                    cout << i << " - " << region[i].get_region_name() << endl;

                cout << region.size() << " - Sair do programa" << endl;

                cout << "\nEscolha: ";
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

            if (esc_region==0) {
                funcInfos(0);
            } else
                do {
                    cout << "\n[ SISTEMA DE ACOMPANHAMENTO DO COVID NO BRASIL ]" << endl;
                    cout << "[ REGIAO " << region[esc_region].get_region_name() << " ]\n" << endl;
                    if (repeat)
                        cout << "Escolha uma opcao valida!\n" << endl;
                    cout << "Qual estado deseja analisar?" << endl;
                    
                    for(int i=0;i<(region[esc_region].state).size();i++)
                        cout << i << " - " << (region[esc_region].state[i]).get_state_name() << endl;
                    
                    cout << (region[esc_region].state).size() << " - Voltar para o menu inicial" << endl;
                    cout << (region[esc_region].state).size()+1 << " - Sair do programa" << endl;

                    cout << "\nEscolha: ";
                    cin >> esc_state_aux; // Escolha do usuário
                    try {
                        esc_state = stoi(esc_state_aux);
                        
                        if(esc_state==(region[esc_region].state).size())
                            break;
                        if(esc_state==(region[esc_region].state).size()+1){
                            isEnd = true;
                            break;
                        }
                        else 
                            if (esc_state<0 || esc_state>(region[esc_region].state).size()+1)
                                repeat = true;
                            else{
                                escCounty(); // Analisa os municipios de um determinado estado 
                                repeat = false;
                                break;
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
                cout << "\n[ SISTEMA DE ACOMPANHAMENTO DO COVID NO BRASIL ]" << endl;
                cout << "[ REGIAO " << region[esc_region].get_region_name() << " ]" << endl;
                cout << "[ ESTADO " << (region[esc_region].state[esc_state]).get_state_name() << " ]\n" << endl;
                if (repeat)
                    cout << endl << "Escolha uma opcao valida!\n" << endl;
                cout << "Qual municipio deseja analisar?" << endl;
                
                for(int i=0;i<((region[esc_region].state[esc_state]).county).size();i++)
                    cout << i << " - " << ((region[esc_region].state[esc_state]).county[i]).get_county_name() << endl;
                
                cout << ((region[esc_region].state[esc_state]).county).size() << " - Analisar o proprio estado " 
                << (region[esc_region].state[esc_state]).get_state_name() << endl;
                cout << ((region[esc_region].state[esc_state]).county).size()+1 
                << " - Voltar para o menu inicial" << endl;
                cout << ((region[esc_region].state[esc_state]).county).size()+2 << " - Sair do programa" << endl;
                
                cout << "\nEscolha: ";
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
                            if(esc_county==((region[esc_region].state[esc_state]).county).size()) 
                                funcInfos(1); // Analisa os dados do próprio estado
                            else {
                                funcInfos(2); // Analisa os dados de um dado municipio
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
        // Recebe Info de um local, uma data inicial e uma data final e plota o histograma
        void plot_histogram (vector <Info>& info, int esc) {  
            float altura_plot = 20;
            int tam = 14;

            int inicio;

            string data_inic;

            data_inic = lerData(true);

            vector <float> dados;

            for (int i=0;i<info.size();i++) 
                if (data_inic == info[i].data) {
                    inicio = i;
                    break;
                }

            cout << endl;
            switch (esc) {
                    case 0:
                        cout << "Casos acumulados por data:\n" << endl;
                        break;
                    case 1:
                        cout << "Casos novos por data:\n" << endl;
                        break;
                    case 2:
                        cout << "Obitos acumulados por data:\n" << endl;
                        break;
                    case 3:
                        cout << "Obitos novos por data:\n" << endl;
                        break;
                    default:
                        break;
            }

            for (int i=0;i<14;i++) {
                switch (esc) {
                    case 0:
                        dados.push_back(info[inicio+i].casosAcumulado);
                        break;
                    case 1:
                        dados.push_back(info[inicio+i].casosNovos);
                        break;
                    case 2:
                        dados.push_back(info[inicio+i].obitosAcumulado);
                        break;
                    case 3:
                        dados.push_back(info[inicio+i].obitosNovos);
                        break;
                    default:
                        break;
                }
                string str = info[inicio+i].data;
                cout << str << " : " << (int) dados[i] << endl;
            }

            float max = *max_element(dados.begin(), dados.end());

            for (int i=0;i<dados.size();i++) 
                dados[i] = altura_plot * dados[i] / (float)(max);

            float delta = 0.5;

            cout << endl;
            cout << endl;

            for (int i=0;i<altura_plot;i++) {

                cout << setw(9) << setfill(' ') << right 
                << ((int)(max*((altura_plot-i)/altura_plot)))<< " " << (char) 179;

                float degrau_piso = altura_plot-i-delta;
                float degrau_teto = altura_plot-i+delta;

                for (int j=0;j<14;j++) {
                    if (dados[j]>=degrau_piso)
                        cout << "   " << BLUE << (char) 219 << RESET << "   "; 
                    else
                        cout << "       ";
                }
                cout << endl;
            }

            cout << "          " << (char) 192;
            for (int i=0;i<14;i++) 
                for (int j=0;j<7;j++)
                    cout << (char) 196;

            cout << endl;
            cout << "           ";
            for (int i=0;i<14;i++) {
                string str = info[inicio+i].data;
                for (int j=0;j<5;j++)
                    str.erase(str.begin());
                cout << " " << str << " ";
            }

            cout << endl;
            cout << endl;
        }
    
        bool histogram (int tipo) {

            bool repeat = false;
            bool endDo = false;
            string esc_aux;
            int esc;

            do { // Menu que recebe as escolhas para dado local
                cout << "\n[ SISTEMA DE ACOMPANHAMENTO DO COVID NO BRASIL ]" << endl;
                cout << "[ REGIAO " << region[esc_region].get_region_name() << " ]" << endl;
                if (tipo>0) {
                    cout << "[ ESTADO " << (region[esc_region].state[esc_state]).get_state_name() << " ]" << endl;
                    if (tipo==1)
                        ptr = &((region[esc_region].state[esc_state]).info);
                    else 
                        cout << "[ MUNICIPIO " << ((region[esc_region].state[esc_state]).county[esc_county]).get_county_name() << " ]" << endl;
                } else
                    ptr = &(region[esc_region].info);
                cout << "[ PLOT DE HISTOGRAMA ]\n" << endl;

                if (repeat)
                cout << "Escolha uma opcao valida!\n" << endl;
                cout << "Deseja plotar o Histograma de 14 dias a frente de:" << endl;
                cout << "0 - Casos acumulados" << endl;
                cout << "1 - Casos novos" << endl;
                cout << "2 - Obitos acumulados" << endl;
                cout << "3 - Obitos novos" << endl;
                cout << "Ou" << endl;
                cout << "4 - Voltar para o menu inicial" << endl;
                cout << "5 - Sair do programa" << endl;

                cout << "\nEscolha: ";
                cin >> esc_aux; // Escolha do usuario
                try {
                    esc = stoi(esc_aux);
                    string data1="", data2="";
                    int aux=0;
                    float auxf=0.0;
                    switch (esc){
                        case 0: // Casos acumulados
                            plot_histogram(*ptr,esc);
                            break;
                        case 1: // Casos novos
                            plot_histogram(*ptr,esc);
                            break;
                        case 2: // Obitos acumulados
                            plot_histogram(*ptr,esc);
                            break;
                        case 3: // Obitos novos
                            plot_histogram(*ptr,esc);
                            break;
                        case 4: // Voltar para o menu inicial
                            endDo = true;
                            break;
                        case 5: // Sair do programa
                            isEnd = true;
                            endDo = true;
                            break;
                        default:
                            repeat = true;
                    }
                    programPause();
                    if (esc>=0 && esc<=5)
                        repeat = false;
                } catch(...) {
                    repeat = true;
                }
            } while (!(endDo));
            return endDo;
        }

        // Realiza todas as manipulações com os dados que o usuário quiser
        void funcInfos(int tipo){
            
            // Passa para o ponteiro o vector <Info> em cada caso
            if (tipo==0)
                ptr = &(region[esc_region].info);
            else {
                if (tipo==1)
                    ptr = &((region[esc_region].state[esc_state]).info);
                else {
                    ptr = &(((region[esc_region].state[esc_state]).county[esc_county]).info);
                }
            }

            bool repeat = false;
            bool endDo = false;
            string esc_aux;
            int esc;
            do { // Menu que recebe as escolhas para dado local
                cout << "\n[ SISTEMA DE ACOMPANHAMENTO DO COVID NO BRASIL ]" << endl;
                cout << "[ REGIAO " << region[esc_region].get_region_name() << " ]" << endl;
                if (tipo>0) {
                    cout << "[ ESTADO " << (region[esc_region].state[esc_state]).get_state_name() << " ]" << endl;
                    if (tipo==1)
                        ptr = &((region[esc_region].state[esc_state]).info);
                    else 
                        cout << "[ MUNICIPIO " << ((region[esc_region].state[esc_state]).county[esc_county]).get_county_name() << " ]" << endl;
                }
                cout << "\n";

                if (repeat)
                    cout << "Escolha uma opcao valida!\n" << endl;
                cout << "0 - Total de casos em determinado intervalo" << endl;
                cout << "1 - Total de obitos em determinado intervalo" << endl;
                cout << "2 - Calculo da media movel de casos em determinado intervalo" << endl;
                cout << "3 - Calculo da media movel de obitos em determinado intervalo" << endl;
                cout << "4 - Calculo da tendencia de crescimento de casos entre duas medias moveis" << endl;
                cout << "5 - Calculo da tendencia de crescimento de obitos entre duas medias moveis" << endl;
                cout << "6 - Plotar histograma" << endl;
                cout << "7 - Voltar para o menu inicial" << endl;
                cout << "8 - Sair do programa" << endl;

                cout << "\nEscolha: ";
                cin >> esc_aux; // Escolha do usuario
                try {
                    esc = stoi(esc_aux);
                    string data1="", data2="";
                    int aux=0;
                    float auxf=0.0;
                    switch (esc){
                        case 0: // Total de casos em determinado intervalo
                            aux = totalDeCasosObitos(esc);
                            cout << "Total de novos casos: " << aux << endl;
                            break;
                        case 1: // Total de obitos em determinado intervalo
                            aux = totalDeCasosObitos(esc);
                            cout << "Total de novos obitos: " << aux << endl;
                            break;
                        case 2: // Cálculo da média móvel de casos em determinado intervalo
                            auxf = mediaMovel(false,0,"");
                            cout << "Media movel de casos: " << auxf << endl;
                            break;
                        case 3: // Cálculo da média móvel de obitos em determinado intervalo
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
                        case 6: // Histograma
                            endDo = histogram (tipo);
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
                    if (!endDo)
                        programPause();
                    if (esc>=0 && esc<=8)
                        repeat = false;
                } catch(...) {
                    repeat = true;
                }
            } while (!(endDo));
        }

        string lerData (bool isPlot=false){
            string data, aux="";
            bool repeat = false;
            int dia, mes, ano;
            bool isDataOk = false;

            string ultima_data_possivel;
            if (isPlot) {
                auto it = ptr->end();
                for (int i=0;i<14;i++) {
                    it--;
                }
                ultima_data_possivel = it->data;
            } else
            ultima_data_possivel = (ptr->back()).data;
            string primeira_data_possivel = (ptr->begin())->data;

            do {
                if (repeat)
                    cout << "\nInsira uma data valida!\n" << endl;
                cout << "Data mais antiga possivel (AAAA/MM/DD): " << primeira_data_possivel << endl;
                cout << "Data mais recente possivel (AAAA/MM/DD): " << ultima_data_possivel << endl;
                cout << "\nEntre com a data desejada no formato (DD/MM/AAAA): ";
                cin >> data;
                aux = textUntilDivision(data,1);
                try {
                    dia = stoi(aux);
                    aux = textUntilDivision(data,1);
                    try {
                        mes = stoi(aux);
                        aux = textUntilDivision(data,1);
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

                                if ((ptr->begin())->data<=data && ultima_data_possivel>=data){
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
            return data;
        }

        int totalDeCasosObitos (int esc) {
            string data1, data2;
            int cont=0;

            string ultima_data_possivel = (ptr->back()).data;
            string primeira_data_possivel = (ptr->begin())->data;

            do {
                if (cont)
                    cout << "\nA data inicial nao pode ser maior que a data final!" << endl;
                cout << "\nInsira as datas inicial e final, respectivamente, para a analise." << endl;
                data1=""; data2="";
                cout << "\nData inicial:" << endl;
                data1 = lerData();
                cout << "\nData final:" << endl;
                data2 = lerData();
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
                        cout << "\nJanela para media movel invalida!\n" << endl;
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
                        cout << "\nJanela para media movel invalida!\n" << endl;
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
                        cout << "\nIntervalo invalido!\n" << endl;
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

            cout << "Media movel mais recente: " << media 
            << "\nMedia movel mais anterior: " << media_ant 
            << "\nVariacao de " << 100*(1-(media/media_ant)) << "%\n" << endl;
            
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

    ifstream File("HIST_PAINEL_COVIDBR_Parte3_20jun2021.csv");

    if (!File.is_open()) {
        cout << "Erro na leitura do arquivo" << endl;
        return -1;
    }
    getline (File, text);

    System t = System(File);

    cout << "Programa finalizado com sucesso!" << endl;

    File.close();
}