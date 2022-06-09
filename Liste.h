#ifndef __LISTE_MAILLON_H
#define __LISTE_MAILLON_H

#include <iostream>
#include <algorithm>

using namespace std;

template <class T>
struct Maillon {
    T val;
    Maillon* suiv;
    Maillon* prec;//si double chainage
};

// Classe Liste
template <class T>
class Liste {
    Maillon<T>* _tete;
    Maillon<T>* _fin;

public:
    // Forme canonique de Coplien (4 operations)
    // Cons. par defaut
    Liste() : _tete(NULL), _fin(NULL) {} //liste vide
    // Cons copie
    Liste(const Liste& l){
        _tete = NULL;
        _fin = NULL;
        Maillon<T>* p = l._fin; //commencer par la fin car on ajoute au debut de la liste
        while(p != NULL){
            ajouter(p->val, 0);
            p = p->prec;
        }
    }
    //constructeur avec liste d'initialisation 
    Liste(string vals){ //fonctionne
        int val_loc;
        int pos = 0;
        _tete = NULL;
        _fin = NULL;
    
        //reverse le string d'input pour pouvoir ajouter a la position 0
        string str = vals;
        reverse(str.begin(), str.end());
        vals = str;

        while(vals.length() >= 0){
            pos = vals.find("-");
            if(pos == -1){ //dernier chiffre
                val_loc = stoi(vals.substr(0));
                this->ajouter(val_loc, 0);
                break;
            }
            else{
                val_loc = stoi(vals.substr(0, pos)); //contient la valeur
                vals.erase(0,pos + 1);
                this->ajouter(val_loc, 0);
            }
        }
    }

    // Destructeur
    ~Liste(){
        delete _tete;
        delete _fin;
    }
    // Affectation
    Liste& operator=(const Liste& l){
        if(this != &l){
            delete _tete;
            delete _fin;
            _tete = NULL;
            _fin = NULL;
            Maillon<T>* p = l._fin;//commencer par la fin car on ajoute au debut de la liste
            while(p != NULL){
                ajouter(p->val, 0);
                p = p->prec;
            }
        }
        return *this;
    }

    // Modificateur
    T& operator[](int pos){
        Maillon<T>* p = _tete;
        for(int i = 0; i < pos; i++){
            p = p->suiv;
        }
        return p->val;
    }

    // Longueur
    int longueur(){
        Maillon<T>* ptrMaillon = _tete;
        int i = 0;
        if(_tete != NULL){
            i = 1; //1er mallion : _tete = _fin
            while(ptrMaillon != _fin){
                i++;
                ptrMaillon = ptrMaillon->suiv;
            }
        }
        return i;
    }

    // Ajout
    void ajouter(T& val, int pos){
        Maillon<T>* ptrMaillon = _tete;
        Maillon<T>* ptrNouveau = new Maillon<T>;
        ptrNouveau->val = val;
        ptrNouveau->suiv = NULL;
        ptrNouveau->prec = NULL;
        if(_tete == NULL){
            _tete = ptrNouveau;
            _fin = ptrNouveau;
        }
        else{
            if(pos == 0){
                ptrNouveau->suiv = _tete;
                _tete->prec = ptrNouveau;
                _tete = ptrNouveau;
            }
            else{
                for(int i = 0; i < pos-1; i++){
                    ptrMaillon = ptrMaillon->suiv;
                }
                ptrNouveau->suiv = ptrMaillon->suiv;
                ptrMaillon->suiv->prec = ptrNouveau;
                ptrMaillon->suiv = ptrNouveau;
                ptrNouveau->prec = ptrMaillon;
            }
            if(pos == longueur()){
                _fin = ptrNouveau;
            }
        }
    }

    // Suppression
    void supprimer(int pos){ //risque que le delete ne fonctionne pas, a tester
        if(0 <= pos <= longueur()){
            if(pos == 0){
                Maillon<T>* ptrMaillon = _tete;
                _tete = ptrMaillon->suiv;
                delete ptrMaillon;
            }
            else{
                Maillon<T>* ptrMaillon = _tete;
                for(int i = 0; i < pos; ++i){
                    ptrMaillon = ptrMaillon->suiv;
                }
                Maillon<T>* ptrMaillonPrec = ptrMaillon->prec;
                Maillon<T>* ptrMaillonSuiv = ptrMaillon->suiv;
                ptrMaillonPrec->suiv = ptrMaillonSuiv;
                ptrMaillonSuiv->prec = ptrMaillonPrec;
                delete ptrMaillon;
            }
        }    
    }

    // Concatenation
    Liste concat(Liste& l){
        Liste l2(*this);
        l2.autoConcat(l);
        return l2;
    }

    // Auto-concatenation
    Liste& autoConcat(Liste& l){
        if(_tete != NULL){ //si la liste n'est pas vide
            Maillon<T>* ptrMaillon = _fin; //commencer a la fin de *this
            for(int i = 0; i < l.longueur(); i++){
                ptrMaillon->suiv = new Maillon<T>;
                ptrMaillon->suiv->val = l[i];
                ptrMaillon->suiv->prec = ptrMaillon;
                ptrMaillon = ptrMaillon->suiv;
                _fin = ptrMaillon;
            }
        }
        else{ //si la liste est vide
            *this = l;
        }
        return *this;
    }

    // Entrees-sorties
    friend ostream& operator<<(ostream &o, Liste &l){
        for(int i = 0; i < l.longueur(); i++){
            o << l[i];
            if(i != l.longueur()-1){
                o << "-";
            }
        }
        return o;
    }

    class ListeIterateur{
        public :
            Maillon<T>* curr;
            Liste* l;
        
        public :
            ListeIterateur(Liste& l1) : l(&l1), curr(l1._tete){}

            void reset(){
                curr = l->_tete;
            }

            bool hasNext(){
                if(l->_tete != NULL){
                    return curr->suiv != NULL;
                }
                else{ //liste vide
                    return false;
                }
            }

            T& getCurrentElement(){
                return curr->val;
            }

            void moveNext(){
                curr = curr->suiv;
            }

            T& getNext(){
                T& val = curr->val;
                curr = curr->suiv; 
                return(val);
            }
    };
};

#endif //__LISTE_MAILLON_H
