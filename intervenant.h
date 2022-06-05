#include<string.h>
#include<iostream>
class Intervenant{
    private:
        int id;
        std::string competence;
        std::string specialite;
        int tps_hebdo;

    public:
        Intervenant(){};
        ~Intervenant(){};
        void SetId(int id){
            this->id = id;
        };
        int GetId(){
            return this->id;
        };
        void SetCompetence(std::string competence){
            this->competence = competence;
        };
        std::string GetCompetence(){
            return this->competence;
        };
        void SetSpecialite(std::string specialite){
            this->specialite = specialite;
        };
        std::string GetSpecialite(){
            return this->specialite;
        };
        void SetTpsHebdo(int tps_hebdo){
            this->tps_hebdo = tps_hebdo;
        };
        int GetTpsHebdo(){
            return this->tps_hebdo;
        };
};
