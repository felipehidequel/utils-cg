/**
 * \file raio.cpp
 *
 * \brief Este arquivo contem os calculos que foram definidos no arquivo raio.hpp, sendo este responsavel pela definicoes de funcoes de interseccoes
 * com os objetos da cena que serao renderizados. Foi utilizado o texto [introducao ao ray tracing] e [RAY TRACING] como base para a implementacao 
 * do arquivo.
 * [Texto de introducao ao ray tracing]: www.visgraf.impa.br/Data/RefBib/PS_PDF/silva94/rt.pdf "introducao ao ray tracing"
 * [Texto de apoio para a disciplina - RAY TRACING]: http://disciplinas.ist.utl.pt/leic-cg/textos/livro/Ray%20Tracing.pdf "RAY TRACING"
 *
 * \author 
 * Petrucio Ricardo Tavares de Medeiros \n
 * Universidade Federal do Rio Grande do Norte \n
 * Departamento de Computacao e Automacao Industrial \n
 * petrucior at gmail (dot) com
 *
 * \version 0.4
 * \date Outubro 2013
 */
 
#include "raio.hpp"		//rayTracing::Raio
#include <iostream>		//min
#include <math.h>		//sqrt

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
   * \fn Raio::Raio();
   *
   * \brief Construtor da classe.
   */
  Raio::Raio(){
    lkf = new Vetor(); ///< Lookfrom
    lkt = new Vetor(); ///< Lookat
    c_esf = new Vetor(); ///< Centro da esfera
    drt = new Vetor(); ///< Vetor Diretor
  }
		
  /**
   * \fn void Raio::atualizar_vetores(Vetor* _lkf, Vetor* _lkt, Objeto* esfera)
   *
   * \brief Atualiza os valores dos vetores de posicao da camera (lookfrom), posicao do centro da esfera e do lookat
   *
   * \param _lkf - vetor de posicao da camera
   * \param _lkt - vetor de posicao do lookat
   * \param esfera - objeto esfera
   */
  void Raio::atualizar_vetores( Vetor* _lkf, Vetor* _lkt, Objeto* esfera)
  {
    //lookfrom
    lkf = _lkf;
    lkfx = _lkf->vx();
    lkfy = _lkf->vy();
    lkfz = _lkf->vz();
		
    //lookat
    lkt = _lkt;
    lktx = _lkt->vx();
    lkty = _lkt->vy();
    lktz = _lkt->vz();
		
    //centro da esfera
    c_esf = esfera->posicao_esfera();
    c_esfx = c_esf->vx();
    c_esfy = c_esf->vy();
    c_esfz = c_esf->vz();
		
    //raio da esfera
    r = esfera->raio();
		
    //vetor diretor
    drt->valores_vetor(0.0, 0.0, 0.0);
  }

  /**
   * \fn void Raio::vetor_diretor()
   *
   * \brief Calcula a diferenca entre o vetor lookat e o vetor lookfrom.
   */
  void Raio::vetor_diretor(){
    drt->vetor_diretor(lkt, lkf);
  }

  /**
   * \fn double Raio::delta(double a, double b, double c)
   *
   * \brief calcula o delta para ser utilizado no metodo de interseccao com a esfera
   *
   * \param a - o valor do a da equacao do segundo grau
   * \param b - o valor do b da equacao do segundo grau
   * \param c - o valor do c da equacao do segundo grau
   *
   * \return o valor de delta
   */
  double Raio::delta(double a, double b, double c){
    return (b*b) - (4*a*c);
  }

  /**
   * \fn double calcula_t() 
   *
   * \brief Atraves de uma manipulacao algebrica apresentada a seguir foi encontrada uma equacao que facilita a descoberta dos valores de t' e t''.
   * Pode ser encontrado o vetor diretor atraves do metodo acima que me retornara uma equacao de valores da forma: \f$ (D_x) = (lkt_x - lkf_x) \f$,
   * \f$(D_y) = (lkt_y - lkf_y) \f$ e \f$ (D_z) = (lkt_z - lkf_z) \f$. Encontramos a equacao parametrizada: \f$ x = (lkf_x) + t(D_x) \f$, \f$ y = (lkf_y) + t(D_y) \f$ e
   * \f$ z = (lkf_z) + t(D_z) \f$. Aplicando na equacao da esfera: \f$ (x - x_0)^2 + (y - y_0)^2 + (z - z_0)^2 = r^2 \f$ o que resultou em:
   * \f$ x^2 - 2x(x_0) + (x_0)^2 + y^2 - 2y(y_0) + (y_0)^2 + z^2 - 2z(z_0) + (z_0)^2 - r^2 = 0 \f$
   * \f$ ((lkf_x) + t(D_x))^2 - 2((lkf_x) + t(D_x))(x_0) + (x_0)^2 + ((lkf_y) + t(D_y))^2 - 2((lkf_y) + t(D_y))(y_0) + (y_0)^2 + ((lkf_z) + t(D_z))^2 - 2((lkf_z) + t(D_z))(z_0) + (z_0)^2 - r^2 = 0 \f$
   * \f$ t^2*((D_x)^2 + (D_y)^2 + (D_z)^2) + t*(2*(lkf_x)*(D_x) + 2*(lkf_y)*(D_y) + 2*(lkf_z)*(D_z) - 2*(c_esf_x)(D_x) - 2*(c_esf_y)(D_y) - 2*(c_esf_z)(D_z)) +(lkf_x)^2 +(lkf_y)^2 +(lkf_z)^2 +(c_esf_x)^2 +(c_esf_y)^2 + +(c_esf_z)^2 - r^2 - 2*(c_esf_x)*(lkf_x) - 2*(c_esf_y)*(lkf_y) - 2*(c_esf_z)*(lkf_z) = 0
   * desta forma para encontrar t' e t'' podemos utilizar a formula de baskara.
   * A partir da equacao acima encontramos a, b e c para ser aplicado a formula de baskara, mas sabemos da f�rmula de baskara que necessitamos 
   * calcular primeiro o \f$ delta = b^2 - 4ac \f$ e tomar conclusoes se intercepta o objeto ou nao e se sim calcular a posicao de interceccao.
   * Nas equacoes D e o vetor diretor, lkf(lookfrom), lkt(lookat) c_esf(centro da esfera).
   *
   * \return o menor valor entre t' e t'' ou um valor -1.0 que implica dizer que nao foi interceptado nenhuma esfera. e necessario saber que se o menor
   * valor e negativo e porque a c�mera esta dentro do objeto.
   */
  double Raio::calcula_t(){
    //Chamando o metodo para atualizar o vetor diretor.
    vetor_diretor();
		
    //Pegando valores do vetor diretor, pois e o unico que no processo de atualizacao foi passado para a estrutura de dados
    drtx = drt->vx();
    drty = drt->vy();
    drtz = drt->vz();
		
    double a = ((drtx)*(drtx)) + ((drty)*(drty)) + ((drtz)*(drtz));
    double b = 2*(lkfx)*(drtx) - 2*(c_esfx)*(drtx) + 2*(lkfy)*(drty) - 2*(c_esfy)*(drty) + 2*(lkfz)*(drtz) - 2*(c_esfz)*(drtz);
    double c = ((lkfx)*(lkfx)) + ((lkfy)*(lkfy)) + ((lkfz)*(lkfz)) + ((c_esfx)*(c_esfx)) + ((c_esfy)*(c_esfy)) + ((c_esfz)*(c_esfz)) - (r*r) - 2*(c_esfx)*(lkfx) - 2*(c_esfy)*(lkfy) - 2*(c_esfz)*(lkfz);
	
    //Encontrando o delta
    double _delta = delta(a, b, c);
    if (_delta < 0){
      return -1.0;
    }
    else{
      double t1 = ( (-1)*(b) + sqrt(_delta) ) / (2 * a);
      double t2 = ( (-1)*(b) - sqrt(_delta) ) / (2 * a);
      return std::min(t1, t2);
    }
  }

  /**
   * \fn Vetor* interseccao_esfera(double t)
   *
   * \brief Atraves do valor de t pode ser encontrado a posicao de interseccao do raio com a esfera aplicando a equacao parametrizada. Sabemos que
   * o vetor diretor esta na forma \f$(D_x) = (lkt_x - lkf_x) \f$, \f$(D_y) = (lkt_y - lkf_y) \f$ e \f$ (D_z) = (lkt_z - lkf_z) \f$ aplicando a equacao parametrizada a qual
   * conhecemos \f$ x = (lkf_x) + t(D_x) \f$, \f$ y = (lkf_y) + t(D_y) \f$ e \f$ z = (lkf_z) + t(D_z) \f$.
   *
   * \param t - valor do t da equacao parametrizada
   *
   * \return a posicao de interseccao entre o raio e a esfera
   */
  Vetor* Raio::interseccao_esfera(double t){
    Vetor* aux = new Vetor();
    aux-> valores_vetor((lkfx + (t * drtx)), (lkfy + (t * drty)), (lkfz + (t * drtz)));
    return aux;
  }

} //Fim do namespace rayTracing
 
/** @} */ //Fim do grupo class
