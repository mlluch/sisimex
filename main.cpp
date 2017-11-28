// SiSiMEX little v1.0 Novembre 2017 - main.cpp
// Autor (codi base): Magí Lluch-Ariet
// Autor (adaptació): 
// Llicència: LGPL v3 
//
// Simulació del Sistema de Suport a l'Intercanvi Multilateral d'Elements en Xarxa (SiSiMEX)
// Versió acadèmica basada en el projecte MOSAIC

#include <iostream>
#include "sisimex.h"
#include "time.h"

void camiNet(Acord *c) {
	int i;

	c->llargada = 0;
	for (i = 0; i < 10; i++) c->node[i] = 0;
	c->owner = nullptr;
	}

void activarMCP(Node *n, char element, Acord *cami,int inicial) {

	n->mcp[n->numMCP].request = element;
	n->mcp[n->numMCP].owner = n;
	n->mcp[n->numMCP].cami = cami;
	if (inicial) { // pel primer MCP inicialitzem el camí de l'acord amb el node del MCP
		n->mcp[n->numMCP].cami->llargada = 1;
		n->mcp[n->numMCP].cami->node[0] = n->id;
		n->mcp[n->numMCP].cami->owner = &(n->mcp[n->numMCP]);
		}
	n->numMCP++;
	}

void activarMCC(YellowPages *y, Node *n, char oferta, char restriccio) {
	n->mcc[n->numMCC].offer = oferta;
	n->mcc[n->numMCC].constraint = restriccio;
	y->mcc[y->numMCC] = &(n->mcc[n->numMCC]);
	n->numMCC = n->numMCC + 1;
	y->numMCC = y->numMCC + 1;
	}

void activarNode(Node *n,int identificador,char element) {
	int i;

	n->id = identificador;
	n->host = element;
	n->numMCC = 0;
	n->numMCP = 0;
	for (i = 0; i < 10; i++) {
		n->mcp[i].request = ' ';
		n->mcp[i].owner = n;
		n->mcc[i].offer = ' ';
		n->mcc[i].constraint = ' ';
		n->mcc[i].owner = n;
		}
	}

int ConsultarYellowPages(YellowPages yp, MCC *mcc[10], char request) {

	int i,j, candidats;
	candidats = 0;
	j = 0;

	for (i = 0; i < yp.numMCC; i++) {
		if (yp.mcc[i]->offer == request) {
			mcc[j++] = yp.mcc[i];
			candidats++;
			}
		}
	return candidats;
	}

int loop(Acord *cami, MCC *mcc) {
	int i = 0, trobat = 0;
	while (!trobat && (i < (cami->llargada-1))) {
		if (cami->node[i++] == mcc->owner->id) trobat = 1;
		}
	return trobat;
	}

int explorar(MCP mcp, YellowPages y) {
	int acord=0;
	int candidats;         // quantitat de MCC que tenen el que busquem
	MCC *mccCandidats[10]; // llista d'apuntadors a MCC que tenen el que busquem
	MCC *mcc;              //MCC seleccionat de tots els candidats

	int index;

	srand(time(NULL));

	candidats=ConsultarYellowPages(y, mccCandidats, mcp.request);
	if (candidats > 0) {
		
		mcc = mccCandidats[rand()%(candidats)]; // Tria aleatòria del camí d'exploració 
		
		mcp.cami->node[mcp.cami->llargada] = mcc->owner->id;
		mcp.cami->llargada++;

		if (mcc->constraint == mcp.owner->host) {
			mcp.cami->node[mcp.cami->llargada++] = mcp.owner->id; // Tanquem l'acord multilateral amb el node inicial
			acord = 1;
			}
		else if (loop(mcp.cami, mcc)) { // MCC candidat ja pertany al camí construit del pre-acord. Tanquem acord.
			std::cout << std::endl << "<Loop detectat>" << std::endl;
			std::cout << "Node "<<mcp.owner->id<<": Petitioner  "<<mcp.owner->numMCP <<" Request: " << mcp.request << std::endl;
			std::cout << "Node "<<mcc->owner->id<<": Contributor " << mcc->owner->numMCC << " Offer:   " << mcc->offer << " (aquest contributor ja formava part del pre-acord construit)" << std::endl;
			acord = 1;
			}
		else {
			activarMCP(mcp.owner, mcc->constraint, mcp.cami,0);
			acord = explorar(mcp.owner->mcp[mcp.owner->numMCP-1], y); // Activació recursiva de l'exploració de la xarxa
			}
		}
	return acord;
	}

void visualitzarXarxa(Node n[],int nombre) { // Funció per la visualització de la xarxa
	int i;

	std::cout << "Sistema de Suport a l'Intercanvi Multilateral d'Elements en Xarxa" << std::endl;
	std::cout << "SiSiMEX little v1.0" << std::endl << std::endl;

	std::cout << "Nodes:      [";
	for (i = 0; i < (nombre - 1); i++) std::cout << n[i].id << ",";
	std::cout << n[i].id << "]";
	std::cout << std::endl;

	std::cout << "Host:       [";
	for (i = 0; i < (nombre - 1); i++) std::cout << n[i].host<< ",";
	std::cout << n[i].host << "]";
	std::cout << std::endl << std::endl;

	std::cout << "Contributor:[";
	for (i = 0; i < (nombre - 1); i++) std::cout << n[i].numMCC << ",";
	std::cout << n[i].numMCC << "]";
	std::cout << std::endl;

	std::cout << "Offer:      ["; // Només visualitza el que ofereix el primer MCC de cada Node (!)
	for (i = 0; i < (nombre - 1); i++) std::cout << n[i].mcc[0].offer<< ",";
	std::cout << n[i].mcc[0].offer << "]";
	std::cout << std::endl;

	std::cout << "Constraint: ["; // Només visualitza el que reclama el primer MCC de cada Node (!)
	for (i = 0; i < (nombre - 1); i++) std::cout << n[i].mcc[0].constraint << ",";
	std::cout << n[i].mcc[0].constraint << "]";
	std::cout << std::endl << std::endl;

	std::cout << "Petitioner: [";
	for (i = 0; i < (nombre - 1); i++) std::cout << n[i].numMCP << ",";
	std::cout << n[i].numMCP << "]";
	std::cout << std::endl;

	std::cout << "Request:    ["; // Només visualitza el que demana el primer MCP de cada Node (!)
	for (i = 0; i < (nombre - 1); i++) std::cout << n[i].mcp[0].request << ",";
	std::cout << n[i].mcp[0].request << "]";
	std::cout << std::endl << std::endl;
}

void visualitzarAcords(MCP mcp) { // Funció per la visualització dels acord multilaterals trobats
	int i;

	std::cout << std::endl << "Acord (nodes): [";

	for (i = mcp.cami->llargada -1 ; i>0; i--) std::cout << mcp.cami->node[i] << ":";
	std::cout << mcp.cami->node[i] << "]";;
	std::cout << std::endl << std::endl;
	}

void main() { // Codi principal, on s'inicialitza la xarxa, s'activen els Agents MCC i MCP i la cerca del acords multilaterals
	
	Node node[6];   // Els 6 nodes de la xarxa en la que farem la simulació
	YellowPages yp; // Serevei de directori (on registrarem els MCC actius)
	Acord cami;     // Nodes que formen part de l'acord multilateral
	int acord=0;    // Indica si l'exploració ha estat satisfactòria
	int root = 1;   // inidica que estem a l'inici de l'exploració (activació primària de MCP)

	yp.numMCC = 0;  // Inicialitza les YellowPages
	camiNet(&cami); // Inicialitza el camí

	activarNode(&node[0], 1,'E'); // Node 1
	activarNode(&node[1], 2,'A'); // Node 2
	activarNode(&node[2], 3,'B'); // Node 3
	activarNode(&node[3], 4,'C'); // Node 4
	activarNode(&node[4], 5,'D'); // Node 5
	activarNode(&node[5], 6,'D'); // Node 6

	activarMCC(&yp, &node[1], 'A', 'B');  // MCC1
	activarMCC(&yp, &node[2], 'B', 'C');  // MCC2
	activarMCC(&yp, &node[3], 'C', 'D');  // MCC3
	activarMCC(&yp, &node[4], 'D', 'E');  // MCC4
	activarMCC(&yp, &node[5], 'D', 'B');  // MCC5
	activarMCP(&node[0], 'A',&cami,root); // MCP1
	
	visualitzarXarxa(node, 6);
	std::cout << "Exploracio amb seleccio aleatoria del cami (torna'm a executar per intentar trobar cami alternatiu)" << std::endl;

	acord=explorar(node[0].mcp[0],yp); // Activació de l'exploració de la xarxa pel 1r MCP del Node1

	if (acord) visualitzarAcords(node[0].mcp[0]);
	else std::cout << "No s'ha trobat cap acord multilateral en aquesta exploració" << std::endl;
	
	system("pause");
	}
