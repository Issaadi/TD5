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
lireDonneesImage ( fstream& fichier, Image& image )
{
	// TODO: Se positionner au début du tableau de pixels dans le fichier.
	fichier.seekg(sizeof(EnteteBmp));
	fichier.seekg(sizeof(EnteteDib));
	
	// TODO: Pour chaque ligne de l'image, lire la ligne et sauter le padding.
	int taillePadding = calculerTaillePadding(image);

	fichier.read((char*)&image.pixels, image.largeur-taillePadding);
}


void
ecrireDonneesImage ( fstream& fichier, const Image& image )
{
	// TODO: Se positionner au début du tableau de pixels dans le fichier (après
	//       les entêtes).
	
	// TODO: Pour chaque ligne de l'image, écrire la ligne puis écrire des bytes
	//       à zéro pour le padding.
}


void
ecrireImage ( const string& nomFichier, const Image& image, bool& ok )
{
	// TODO: Ouvrir un fichier en écriture binaire.
	
	// Si l'ouverture n'a pas échouée :
		// TODO: Construire les entêtes à partir de l'image.
		
		// TODO: Écrire les entêtes dans le fichier.
		
		// TODO: Écrire les données (pixels) de l'image dans le fichier.
}


Image
allouerImage ( unsigned largeur, unsigned hauteur )
{
	// Si la largeur ET la hauteur ne sont pas nulles :
		// TODO: Affecter les dimensions à l'image.
		
		// TODO: Allouer un tableau dynamique de pixels pour l'image.
		//       On veut Image::hauteur de lignes qui ont chacune
		//       Image::largeur de pixels.
	return {}; // TODO: Retourner ce qu'il faut.
}


void
desallouerImage ( Image& image )
{
	// Si le tableau dynamique de l'image n'est pas nul :
		// TODO: Désallouer le tableau 2D.
}


Image
copierImage ( const Image& image )
{
	// TODO: Allouer une image de la même taille que celle donnée.
	
	// TODO: Copier tous les pixels.
	return {}; // TODO: Retourner ce qu'il faut.
}


Image
lireImage ( const string& nomFichier, bool& ok )
{
	// TODO: Ouvrir le fichier en lecture binaire.
	
	// Si l'ouverture n'a pas échouée :
		// TODO: Lire l'entête DIB.
		
		// TODO: Allouer une image selon le contenu du DIB.
		
		// TODO: Lire les pixels du fichier.
	return {};  // TODO: Retourner ce qu'il faut.
}


Image
extraireRectangle ( const Image& image, const Rectangle& zone )
{
	// Si la zone demandée est valide :
		// TODO: Allouer une image de la taille de la zone à extraire.
		
		// TODO: Copier les pixels de la zone.
	return {};  // TODO: Retourner ce qu'il faut.
}

#pragma endregion //}

#pragma endregion //}

