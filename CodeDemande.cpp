////////////////////////////////////////////////////////////////////////////////
/// VOTRE ENTÊTE ICI
////////////////////////////////////////////////////////////////////////////////


#pragma region "Inclusions" //{

#include "CodeFourni.hpp"

#include "CodeDemande.hpp"

#include <ciso646>
#include <cstddef>
#include <cstdint>
	  
#include <fstream>
#include <string>

#include "gsl/span"
#include "cppitertools/range.hpp"
#include <iostream>

using namespace std;
using namespace gsl;
using namespace iter;

#pragma endregion //}




#pragma region "Globaux" //{

#pragma region "Fonctions" //{

EnteteDib
lireEnteteFichier ( fstream& fichier )
{
	
	// TODO: Lire l'entête DIB du fichier donné et la retourner.

	EnteteDib enteteDib;

	fichier.seekg(sizeof(EnteteBmp));
	fichier.read((char*)& enteteDib, sizeof(EnteteDib));
	return enteteDib;
}


void
lireDonneesImage(fstream& fichier, Image& image)
{
	// TODO: Se positionner au début du tableau de pixels dans le fichier.
	fichier.seekg(sizeof(EnteteBmp) + sizeof(EnteteDib));

	// TODO: Pour chaque ligne de l'image, lire la ligne et sauter le padding.
	span <Pixel*> pixelLigne(image.pixels, image.hauteur);
	unsigned sizeOfLine = image.largeur * sizeof(Pixel);
	
	unsigned taillePadding = calculerTaillePadding(image);

	for (Pixel* colonneP : pixelLigne) {

		fichier.read((char*)colonneP, sizeOfLine);
		fichier.seekg(taillePadding, ios::cur);
	}
	
}


void
ecrireDonneesImage ( fstream& fichier, const Image& image )
{
	// TODO: Se positionner au début du tableau de pixels dans le fichier (après
	//       les entêtes).
	fichier.seekp(sizeof(EnteteBmp)+ sizeof(EnteteDib), ios::beg);
	
	// TODO: Pour chaque ligne de l'image, écrire la ligne puis écrire des bytes
	//       à zéro pour le padding.
	span <Pixel*> pixelColonne(image.pixels, image.hauteur);
	unsigned sizeOfLine = image.largeur * sizeof(Pixel);
	unsigned paddingSize = calculerTaillePadding(image);
	uint32_t valeur = 0;
	for (Pixel* lignePixel : pixelColonne) {
			fichier.write((const char*)lignePixel, sizeOfLine);
			fichier.write((const char*)&valeur, paddingSize);
	}
	

	 //fichier.close();
}


void
ecrireImage(const string& nomFichier, const Image& image, bool& ok)
{
	
	// TODO: Ouvrir un fichier en écriture binaire.
	fstream fichier;

	fichier.open(nomFichier, ios::out | ios::binary );
	
	// Si l'ouverture n'a pas échouée :
	ok = !fichier.fail();
	if ( !ok ) {
		fichier.open(nomFichier, ios::out | ios::binary | ios::trunc);
		if (fichier.fail()) {
			cout << boolalpha << !ok << endl;
		}
		else cout << boolalpha << ok << endl;
		return;
	}
	  
		// TODO: Construire les entêtes à partir de l'image.
	EnteteBmp bmp = construireEnteteBmp(image);
	EnteteDib dib = construireEnteteDib(image);
		// TODO: Écrire les entêtes dans le fichier.
	fichier.write((char*)&bmp, sizeof(EnteteBmp));
	fichier.write((char*)&dib, sizeof(EnteteDib));
		// TODO: Écrire les données (pixels) de l'image dans le fichier.
	ecrireDonneesImage(fichier, image);
}


Image
allouerImage ( unsigned largeur, unsigned hauteur )
{
	// Si la largeur ET la hauteur ne sont pas nulles :
		// TODO: Affecter les dimensions à l'image.
	Image image = {};
	if (largeur && hauteur != 0) {

		image.largeur = largeur;
		image.hauteur = hauteur;

		// TODO: Allouer un tableau dynamique de pixels pour l'image.
		//       On veut Image::hauteur de lignes qui ont chacune
		//       Image::largeur de pixels.
		Pixel** pixel = new Pixel* [hauteur];

		for (int x : range(hauteur)) {
			pixel[x] = new Pixel[largeur];
			
		}
		image.pixels = pixel;
	}
	return {image}; // TODO: Retourner ce qu'il faut.
}


void
desallouerImage ( Image& image )
{
	// Si le tableau dynamique de l'image n'est pas nul :
		// TODO: Désallouer le tableau 2D.
	/*span <Pixel*> pixelLigne(image.pixels, image.hauteur);
	
	for (Pixel* ligneCol : pixelLigne) {
		if (image.pixels[pixelLigne.at] != nullptr) {
			delete[] image.pixels[pixelLigne.at];
			delete[] image.pixels;
			image.pixels = NULL;
		}
		
	}
	*/

	for (int i : range(image.hauteur)) {
		if (image.pixels[i] != nullptr) {
			delete[] image.pixels[i];
			image.pixels[i] = 0;
		}
	}
	
}


Image
copierImage ( const Image& image )
{
	// TODO: Allouer une image de la même taille que celle donnée.
	Image imageC = allouerImage(image.largeur, image.hauteur);
	// TODO: Copier tous les pixels.
	for (int i : range(image.hauteur)) {
		for (int j : range(image.largeur)) {
			imageC.pixels[i][j] = image.pixels[i][j];
		}
	}
	return imageC; // TODO: Retourner ce qu'il faut.
}


Image
lireImage ( const string& nomFichier, bool& ok )
{
	// TODO: Ouvrir le fichier en lecture binaire.
	fstream fichier;
	Image imageC={};
	fichier.open(nomFichier, ios::in | ios::binary);
	ok = !fichier.fail();
	// Si l'ouverture n'a pas échouée :
	if (!ok) {
		cout << "la lecture du fichier a echouee";
	}else{
		// TODO: Lire l'entête DIB.
		EnteteDib enteteDib = lireEnteteFichier(fichier);

		// TODO: Allouer une image selon le contenu du DIB.
		imageC = allouerImage(enteteDib.largeurImage, enteteDib.hauteurImage);

		// TODO: Lire les pixels du fichier.
		lireDonneesImage(fichier, imageC);
	}
	
	return imageC;  // TODO: Retourner ce qu'il faut.
}


Image
extraireRectangle ( const Image& image, const Rectangle& zone )
{
	Image imageA = {};
	// Si la zone demandée est valide :
	if (estZoneValide(image, zone)) {
		// TODO: Allouer une image de la taille de la zone à extraire.
		imageA = allouerImage(zone.coin2.x - zone.coin1.x, zone.coin2.y - zone.coin1.y);

		// TODO: Copier les pixels de la zone.
		for (unsigned i : range(imageA.hauteur)) {
			for (unsigned j : range(imageA.largeur)) {
				imageA.pixels[i][j] = image.pixels[i + zone.coin1.y][j + zone.coin1.x];
			}
		}
	}
	return imageA;  // TODO: Retourner ce qu'il faut.
}

#pragma endregion //}

#pragma endregion //}

