//-----------------------
//------ INCLUDE --------
//-----------------------

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
using namespace std; // Para funções do 'iostream'
using namespace cv; // Para funções do 'OpenCV'

int main(int argc, char** argv)
{
  Mat origem, destino, interm, transformada;
  int brilho;
  double contraste;

  // Imagem utilizada
  origem = imread( argv[1], 1 );
  
  if( !origem.data ){ return -1; }

  //-----------------------
  //---- TRANSFORMAÇÃO ----
  //-----------------------

  // Obs.: é possível usar a função 'convertTo()' do OpenCV para aplicar
  // a transformação abaixo.

  // Nova imagem do tamanho da original para ser transformada
  transformada = Mat::zeros( origem.size(), origem.type() );

  cout << "\nTransformação Linear: "<<endl;
  cout << "1 - Entre com o valor do contraste [1.0-3.0]: ";
  cin >> contraste;
  cout << "2 - Entre com o valor do brilho [0-100]: ";
  cin >> brilho;
  
  // A nova imagem será submetida a seguinte transformação:
  // g(i,j) = contraste * f(i,j) + brilho
  for( int y = 0; y < origem.rows; y++ ) //Percorre as linhas da imagem
    {for( int x = 0; x < origem.cols; x++ ) // Percorre as colunas da imagem
      {for( int c = 0; c < 3; c++ ) // Para cada valor do pixel em R,G,B
      // Aplica os valores de contraste e brilho escolhidos
        {transformada.at<Vec3b>(y,x)[c] = saturate_cast<uchar>( contraste*( origem.at<Vec3b>(y,x)[c] ) + brilho );}
      // origem.at<Vec3b>(y,x)[c] ==> Acessa o valor do pixel em R,G,B
      // na linha 'y' | coluna 'x'.
      // saturate_cast ==> Garante que caso hajam valores fora do range 0-255,
      // estes sejam convertidos para valores válidos.
    }
  }

  namedWindow("Imagem Original", CV_WINDOW_AUTOSIZE );
  imshow("Imagem Original", origem );

  //-----------------------
  //---- EQUALIZAÇÃO ------
  //-----------------------

  // Função de Conversão para Escalas de Cinza
  cvtColor( origem, interm, CV_BGR2GRAY );

  /* Obs.: é possível realizar o casamento abaixo manualmente aplicando a função normalize()
    do OpenCV na entrada, juntamente com o laço recursivo:

    for (int j=0; j<origemNorm.cols; j++)
      {
        double histo1 = origemNorm.cols(j);

        for (uchar k = ultimo; k<destino.cols; k++)
        {
          double histo2 = destino(k);
          if(abs(histo2 - histo1) < 0.000001 || histo2 > histo1)
          {
            lookup(j) = k;
            ultimo = k;
            break;
          }
        }
      }
  */

  // Função de Equalização do Histograma
  equalizeHist( interm, destino );

  //-----------------------
  //---- HISTOGRAMAS ------
  //-----------------------

  // Separação em R, G e B
  vector<Mat> bgr_planes;
  split( origem, bgr_planes );

  // Número de pontos 0 a 255
  int tamHisto = 256;
  float range[] = { 0, 256 } ;
  const float* histoRange = { range };
  bool uniform = true; bool accumulate = false;

  Mat r_histo, g_histo, b_histo, gray_histo;

  // Cálculo dos histogramas em R,G,B e o White para o Casamento
  calcHist( &bgr_planes[2], 1, 0, Mat(), r_histo, 1, &tamHisto, &histoRange, uniform, accumulate );
  calcHist( &bgr_planes[1], 1, 0, Mat(), g_histo, 1, &tamHisto, &histoRange, uniform, accumulate );
  calcHist( &bgr_planes[0], 1, 0, Mat(), b_histo, 1, &tamHisto, &histoRange, uniform, accumulate );
  calcHist( &destino, 1, 0, Mat(), gray_histo, 1, &tamHisto, &histoRange, uniform, accumulate );  

  // Nova imagem para mostrar o histograma

  int histo_w = 800; int histo_h = 600;
  int bin_w = cvRound( (double) histo_w/tamHisto );

  Mat histoImagem( histo_h, histo_w, CV_8UC3, Scalar( 0,0,0) );

  // Normaliza o resultado para melhor visualização
  normalize(b_histo, b_histo, 0, histoImagem.rows, NORM_MINMAX, -1, Mat() );
  normalize(g_histo, g_histo, 0, histoImagem.rows, NORM_MINMAX, -1, Mat() );
  normalize(r_histo, r_histo, 0, histoImagem.rows, NORM_MINMAX, -1, Mat() );
  normalize(gray_histo, gray_histo, 0, histoImagem.rows, NORM_MINMAX, -1, Mat() );

  // Desenha os histogramas em R,G,B e o White para o Casamento
  for( int i = 1; i < tamHisto; i++ )
  {
      line( histoImagem, Point( bin_w*(i-1), histo_h - cvRound(b_histo.at<float>(i-1)) ) ,
                       Point( bin_w*(i), histo_h - cvRound(b_histo.at<float>(i)) ),
                       Scalar( 255, 0, 0), 1, 8, 0  );
      line( histoImagem, Point( bin_w*(i-1), histo_h - cvRound(g_histo.at<float>(i-1)) ) ,
                       Point( bin_w*(i), histo_h - cvRound(g_histo.at<float>(i)) ),
                       Scalar( 0, 255, 0), 1, 8, 0  );
      line( histoImagem, Point( bin_w*(i-1), histo_h - cvRound(r_histo.at<float>(i-1)) ) ,
                       Point( bin_w*(i), histo_h - cvRound(r_histo.at<float>(i)) ),
                       Scalar( 0, 0, 255), 1, 8, 0  );
      line( histoImagem, Point( bin_w*(i-1), histo_h - cvRound(gray_histo.at<float>(i-1)) ) ,
                       Point( bin_w*(i), histo_h - cvRound(gray_histo.at<float>(i)) ),
                       Scalar( 255, 255, 255), 1, 8, 0  );
  }

  //-----------------------
  //------- SAÍDAS --------
  //-----------------------
  
  namedWindow("Histograma - Original & Equalizada", CV_WINDOW_AUTOSIZE );
  namedWindow("Imagem Transformada", CV_WINDOW_AUTOSIZE );
  namedWindow("Imagem Equalizada em Escala de Cinza", CV_WINDOW_AUTOSIZE );
  imshow("Histograma - Original & Equalizada", histoImagem );
  imshow("Imagem Transformada", transformada );
  imshow("Imagem Equalizada em Escala de Cinza", destino );

  waitKey(0);

  return 0;
}
