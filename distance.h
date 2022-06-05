#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
class Distance{
private:
    double **dist;
    int taille;
public:
    Distance();
    ~Distance(){
        for(int i=0; i<taille; i++){
            delete[] dist[i];
        }
        delete[] dist;
    }
    Distance(char *filename){
        string line, word;
        int count = 0;
        int i = 0;
        //fichier intervenant
        fstream file (filename, ios::in);
        if(file.is_open())
        {
            while(file.peek()!=EOF)
            {
                getline(file, line);
                count++;
            }
            file.close();
        }
        this->dist = new double*[count];
        for(int i = 0; i < count; i++){
            this->dist[i] = new double[count];
        }
        file.open(filename, ios::in);
        if(file.is_open()){
            this->taille = count;
            i = 0;
            while(getline(file, line))
            {
                stringstream str(line);
                for(int j = 0; j < count; j++){
                    getline(str, word, ',');
                    this->dist[i][j] = atof(word.c_str());
                }
                i++;
            }
            file.close();
        }
        else
            cout<<"Could not open the file: " << filename << endl;
    }

    double getDistanceMtoM(int i, int j){
        return this->dist[j][i]; //inversion car la 1ère colonne sont les distance sessad a mission donc colonne i = distance entre i et mission j
    }
    double getDistanceMtoSESSAD(int i){
        return this->dist[0][i];
    }
    double getDistanceSESSADtoM(int i){
        return this->dist[i][0];
    }
};
