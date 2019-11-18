////////////////////////////////////////////////////////////////////////////////
/// VOTRE ENTÊTE ICI
////////////////////////////////////////////////////////////////////////////////


#pragma region "Inclusions" //{

#include "CodeDemande.hpp"
#include "CodeFourni.hpp"

#include <ciso646>
#include <cstddef>
#include <cstdint>

#include <iomanip>
#include <iostream>
#include <string>

#include "cppitertools/range.hpp"

using namespace std;
using namespace iter;

#pragma endregion //}




void executerPartie1 ( );

void executerPartie2 ( );

string construireNomFichierResultat ( const string&, const string& );




int main ( )
{
	cout.setf(ios::boolalpha);

	cout << "---- PARTIE 1 ----" << "\n";

	executerPartie1();

	cout << "\n";

	cout << "---- PARTIE 2 ----" << "\n";

	executerPartie2();

	cout << "\n";

	return 0;
}




/**
 * Effectue des opérations pour tester les fonctions de la partie 1.
 * <p>
 * Le fichier "resultat_partie_1.bmp" devrait correspondre à
 * "flight_line_generator" en noir et blanc.
 */
void executerPartie1 ( )
{
	const unsigned LARGEUR_GENERATEUR = 100;
	const unsigned HAUTEUR_GENERATEUR = 110;
	const string nomFichierGenerateur = "flight_line_generator.bmp";
	const string nomFichierPartie1 = "resultat_partie_1.bmp";

	Pixel tamponMemoire[HAUTEUR_GENERATEUR][LARGEUR_GENERATEUR];
	Pixel* tamponPixels[HAUTEUR_GENERATEUR];
	for ( unsigned i : range(HAUTEUR_GENERATEUR) )
		tamponPixels[i] = tamponMemoire[i];

	// Voici une image de 100x110 déjà allouée pour vous aider à tester vos
	// fonctions sans faire d'allocation dynamique.
	Image imagePartie1 = {LARGEUR_GENERATEUR, HAUTEUR_GENERATEUR,
	                      tamponPixels};

	fstream fichierGenerateur(nomFichierGenerateur, ios::in | ios::binary);
	if ( fichierGenerateur.fail() ) {
		cout << "ERREUR: " << nomFichierGenerateur << " introuvable." << "\n";
		return;
	}

	bool ok;

	EnteteDib dibGenerateur = lireEnteteFichier(fichierGenerateur);

	ok = dibGenerateur.largeurImage == LARGEUR_GENERATEUR and
	     dibGenerateur.hauteurImage == HAUTEUR_GENERATEUR and
	     dibGenerateur.tailleTableau == calculerTailleTableau(imagePartie1);
	cout << "Contenu du DIB OK : " << ok << "\n";

	lireDonneesImage(fichierGenerateur, imagePartie1);

	convertirNoirEtBlanc(imagePartie1);

	ecrireImage(nomFichierPartie1, imagePartie1, ok);
	cout << "Ecriture OK : " << ok << "\n";
}


/**
 * Effectue des opérations pour tester les fonctions de la partie 1.
 * <p>
 * Demande en boucle, jusqu'à l'entrée d'une certaine commande, des coordonnées
 * de rectangle à l'utilisateur, marque ces zones dans "resultat_partie_2.bmp"
 * et les extraits dans des fichiers individuels.
 */
void executerPartie2 ( )
{
	const unsigned EPAISSEUR_TRAIT = 3;

	const string commandeFin = "quit";
	const Pixel noir = {0x00, 0x00, 0x00};

	const string nomFichierOriginal = "flight_line.bmp";
	const string nomFichierPartie2 = "resultat_partie_2.bmp";

	bool ok;

	// TODO: Lire le fichier d'image originale. Le nom du fichier est donné par
	//       «nomFichierOriginal». Vérifier l'ouverture et afficher si celle-ci
	//       a réussie. Quittez si elle a échoué.
	fstream fichier;

	fichier.open(nomFichierOriginal, ios::in | ios::binary);
	ok = !fichier.fail();

	if (!ok) {
		cout << "Erreur de lecture du fichier";
		exit(0);
	}
	else {
		cout << "Lecture du fichier reussie";
		cout << "Entrez '" << commandeFin << "' comme nom d'image pour quitter." << "\n\n";

		// Tant que l'utilisateur n'entre pas «commandeFin» :
		while (true) {

			// TODO: Demander et lire le nom de l'image à extraire.
			string nomImageAExtraire = "", nomFichierExtrait = "";
			Point p1, p2;
			cout << "Nom de l'image a extraire : ";
			cin >> nomImageAExtraire;
			if (nomImageAExtraire == "quit")
				exit(0);
			
			// TODO: Construire le nom de fichier de l'image extraite en utilisant
			//       la fonction construireNomFichierResultat()
			nomFichierExtrait = construireNomFichierResultat(nomFichierPartie2, nomImageAExtraire);

			// TODO: Demander et lire les coins du rectangle à extraire.
			//       ex.: Coin inferieur gauche (x y) : 580 410
			cout << "Coin inferieur gauche (x, y) : ";
			cin >> p1.x >> p1.y;

			cout << "Coin superieur droit (x, y) : ";
			cin >> p2.x >> p2.y;

			Rectangle zone;
			zone.coin1 = p1;
			zone.coin2 = p2;

			// TODO: Extraire le rectangle de l'image original.
			Image imageTotale;
			imageTotale = lireImage(nomFichierOriginal, ok);
			Image imageExtraite = allouerImage(p2.x - p1.x, p2.y - p1.y);
			imageExtraite = extraireRectangle(imageTotale, zone);


			// TODO: Écrire l'image extraite dans un fichier. Afficher la réussite
			//       ou l'échec de l'écriture.
			ecrireImage(nomFichierExtrait, imageExtraite, ok);

			// TODO: Marquer le rectangle extrait sur l'image original (tracer le
			//       contour sur la grande image). Utiliser une ligne noire.
			tracerContourRectangle(imageTotale, noir, zone, 10);

			// TODO: Écrire (écraser) le fichier de la grande image modifiée.
			//       Le nom du fichier est donné par « nomFichierPartie2 ».
			ecrireImage(nomFichierPartie2, imageTotale, ok);
			if (ok)
				cout << "Ecriture de l'image : OK\n\n";

			//desallouerImage(imageExtraite);

		}
		
	}

	// N'oubliez pas de faire les désallocations nécessaires! (dans la boucle et à l'extérieur)
}


/**
 * Construit le nom du fichier pour les résultats d'extraction de la partie 2.
 *
 * \param [in] nomFichierPartie2
 * \param [in] nomImageExtraite
 *
 * \return Le nom du fichier généré.
 */
string construireNomFichierResultat ( const string& nomFichierPartie2, const string& nomImageExtraite )
{
	size_t positionDernierPoint = nomFichierPartie2.rfind('.');
	string extension = nomFichierPartie2.substr(positionDernierPoint);
	size_t longueur = nomFichierPartie2.size() - extension.length();

	return nomFichierPartie2.substr(0, longueur) + "_" + nomImageExtraite + extension;
}
