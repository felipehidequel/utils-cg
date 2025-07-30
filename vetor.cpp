/**
 * \file Vetor.cpp
 *
 * \brief Este arquivo e um pacote que contem as funcoes definidas no arquivo Vetor.hpp, que e responsavel pela definicoes de funcoes da estrutura 
 * dos vetores.
 *
 * \author 
 * Petrucio Ricardo Tavares de Medeiros \n
 * Universidade Federal do Rio Grande do Norte \n
 * Departamento de Computacao e Automacao Industrial \n
 * petrucior at gmail (dot) com
 *
 * \version 0.0
 * \date Outubro 2013
 */

#include "vetor.hpp"	//rayTracing::Vetor
#include <math.h>		//funcoes sqrt e acos
 
/** 
 * \defgroup RayTracingNameSpace Namespace rayTracing.
 * @{
 */
 
/**
 * \namespace rayTracing
 *
 * \brief O espaco de nomes rayTracing contem todas os arquivos que sao utilizados para sua implementacao.
 */
namespace rayTracing{
  //------------------------------
  //	Metodos publicos
  //------------------------------
  /**
   * \fn Vetor::Vetor();
   *
   * \brief Construtor da classe vetor.
   */
  Vetor::Vetor(){
    //Todas as coordenadas estao zeradas
    x = 0;
    y = 0;
    z = 0;
    pi = 3.141592;
  }
	
  /**
   * \fn void Vetor::valores_vetor(double _x, double _y, double _z);
   *
   * \brief Atualiza os valores do vetor.
   *
   * \param _x - valor correspondente a coordenada x
   * \param _y - valor correspondente a coordenada y
   * \param _z - valor correspondente a coordenada z			 
   */
  void 
  Vetor::valores_vetor(double _x, double _y, double _z){
    x = _x;
    y = _y;
    z = _z;
  }
	
  /**
   * \fn double Vetor::vx();
   *
   * \brief Encontra o valor da coordenada x.
   *
   * \return O valor da coordenada x do vetor.
   */
  double 
  Vetor::vx(){
    return x;
  }
	
  /**
   * \fn double Vetor::vy();
   *
   * \brief Encontra o valor da coordenada y.
   *
   * \return O valor da coordenada y do vetor.
   */
  double 
  Vetor::vy(){
    return y;
  }
	
  /**
   * \fn double Vetor::vz();
   *
   * \brief Encontra o valor da coordenada z.
   *
   * \return O valor da coordenada z do vetor.
   */
  double 
  Vetor::vz(){
    return z;
  }
	
  /**
   * \fn void Vetor::vetor_diretor(Vetor& v, Vetor& r);
   *
   * \brief Calcula a diferenca entre vetores.
   */
  void 
  Vetor::vetor_diretor(Vetor* v, Vetor* r){
    x = v->x - r->x;
    y = v->y - r->y;
    z = v->z - r->z;
    return;
  }
	
  /**
   * \fn double Vetor::norma();
   *
   * \brief Calcula a norma ou modulo do vetor.
   *
   * \return A norma do vetor.
   */
  double 
  Vetor::norma(){ 
    double aux = (x*x) + (y*y) + (z*z);
    double value = sqrt(aux);
    return value;
  }
	
  /**
   * \fn double Vetor::produto_escalar(const Vetor& v);
   *
   * \brief Calcula o produto escalar entre os dois vetores.
   *
   * \param v - vetor que esta realizando o produto escalar.
   *
   * \return O valor do produto escalar.
   */
  double 
  Vetor::produto_escalar(Vetor* v){
    double value = (x * v->x) + (y * v->y) + (z * v->z);
    return value;
  }
	
  /**
   * \fn double Vetor::angulo(const Vetor& v);
   *
   * \brief Calcula o angulo entre vetores [em radianos], mas dentro do metodo ha uma conversao para graus.
   *
   * \param v - vetor que se deseja encontrar o angulo.
   *
   * \return O angulo entre os dois vetores em GRAUS.
   */
  double Vetor::angulo(Vetor* v){
    double produtoEscalar = produto_escalar(v);
    double normaA = norma();
    double normaV = v->norma();
    double ang_radianos = acos(produtoEscalar/(normaA * normaV));
    /* 360 graus -----> 2*pi
       ang -----------> ang_radianos */
    double ang = (360.0 * ang_radianos)/(2 * pi);
    return ang;
  }
	
} //Fim do namespace rayTracing
 
/** @} */ //Fim do grupo class
