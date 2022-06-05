#include <string.h>
#include <iostream>
class Mission
{
private:
    int id;
    int jour; //0 à 7
    int debut_min;
    int fin_min;
    std::string competence;
    std::string specialite;
public:
    Mission(){};
    ~Mission(){};
    void SetId(int id){
        this->id = id;
    };
    int GetId(){
        return this->id;
    };
    void SetJour(int jour){
        this->jour = jour;
    };
    int GetJour(){
        return this->jour;
    };
    void SetDebutMissionMinute(int debut_min){
        this->debut_min = debut_min;
    };
    int GetDebutMissionMinute(){
        return this->debut_min;
    };
    void SetFinMissionMinute(int fin_min){
        this->fin_min = fin_min;
    };
    int GetFinMissionMinute(){
        return this->fin_min;
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
};