// SiSiMEX little v1.0 Novembre 2017 - sisimex.h
// Autor (codi base): Mag� Lluch-Ariet
// Autor (adaptaci�): 
// Llic�ncia: LGPL v3 

struct Node;
struct MCP;

struct Acord {
	int llargada; //quantitat de nodes de l'acord
	int node[10]; //nodes que defineixen l'acord trobat pel MCP
	MCP *owner;   // MCP al que est� vinculat l'acord
	};

struct MCP {
	char request; // Element que es busca
	Acord *cami; // Apuntador al conjunt de nodes que formen part de l'acord
	Node *owner; // Node al que pertany
	};

struct MCC {
	char offer; // Element que ofereix el MCC
	char constraint; // Restricci� del MCC per entregar el que ofereix
	Node *owner; // Node propietari del MCC
	};

struct Node {
	int id; // Identificador del Node
	char host; // Element que allotja el node
	int numMCP; // Quantitat de MCP
	int numMCC; // Quantitat de MCC
	struct MCC mcc[10]; // Conjunt de MCC del Node
	struct MCP mcp[10]; // Conjunt de MCP del Node
	};

struct YellowPages {
	int numMCC; // Quantitat de MCC registrats
	MCC *mcc[10]; // Apuntador al MCC
	};
