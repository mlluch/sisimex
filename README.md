# sisimex
Sistema de Suport a l'Intercanvi Multilateral d'Elements en Xarxa
Exemple de funcionament de l’algorisme per la cerca d’acords multilaterals

Descripció de la Xarxa (exemple)
Nodes: N1, ..., N6
Elements d’intercanvi: A, B, C, D, E
Nodes amb MCC: N2, N3, N4, N5 i N6
Nodes que activen un MCP: N1

Identificació dels MCP: IdNode.IdAgent o IdUnicast.IdAgent (ex. N1.MCP1 o UCP1.MCP1)
Identificació dels MCC: IdNode.IdAgent (ex. N1.MCP1 o N2.MCC1)
Identificació dels UCP i UCC: IdNode.IdAgentMulticast.IdAgent (ex. N1.MCP1.UCP1)
Cada MCP té un element que sol·licita (ex. MCP1.Request = A)
Cada MCC té un element que ofereix i un altre que reclama a canvi (ex. N2.MCC.Offer=A, N2.MCC.Constraint=B)
Cada MCP té a la seva disposició els elements que allotja el seu Node (ex. MCP1.Hosts=E)

Els elements de SiSiMEX tenen doncs diferents tractaments o categories:
1.	HOST: Els que estan allotjats en un Node (a disposició dels MCP)
2.	REQUEST: Els que es volen aconseguir per part d’algun MCP
3.	OFFER: Els que els MCC ofereixen
4.	CONSTRAINT: Els que els MCC reclamen com a condició per entregar els que ofereixen

Exemple de configuració
N1.Host=E | N1.MCP.Request=A 
N2.MCC.Offer=A | N2.MCC.Constraint=B
N3.MCC.Offer=B | N3.MCC.Constraint=C
N4.MCC.Offer=C | N4.MCC.Constraint=D
N5.MCC.Offer=D | N5.MCC.Constraint=E
N6.MCC.Offer=D | N5.MCC.Constraint=B

Etapes de construcció dels camins

MCP Activat: MCP1 | MCP1.Request = A | MCP1.Mother=User1 | MCP1.Node=N1
MCC candidat per a resoldre petició: N2.MCC | N2.MCC.Offer=A | N2.MCC.Constraint=B
Constraint resolt: NO | Cal activar nou MCP: SÍ
Acord multilateral: N2.MCC > MCP1 [2,1] 

MCP Activat: MCP2 | MCP2.Request = B | MCP2.Mother=MCP1.UCP1 | MCP2.Node=N1
MCC candidat per a resoldre petició: N3.MCC | N3.MCC.Offer=B | N2.MCC.Constraint=C
Constraint resolt: NO | Cal activar nou MCP: SÍ
Acord multilateral: N3.MCC > N2.MCC > MCP1 [3,2,1]

MCP Activat: MCP3 | MCP3.Request = C | MCP3.Mother=MCP2.UCP1 | MCP3.Node=N1
MCC candidat per a resoldre petició: N4.MCC | N4.MCC.Offer=C | N4.MCC.Constraint=D
Constraint resolt: NO | Cal activar nou MCP: SÍ
Acord multilateral: N4.MCC > N3.MCC > N2.MCC > MCP1 [4,3,2,1]

MCP Activat: MCP4 | MCP4.Request = D | MCP4.Mother=MCP3.UCP1 | MCP4.Node=N1
MCC candidat 1 per a resoldre petició: N5.MCC | N5.MCC.Offer=D | N5.MCC.Constraint=E
Constraint resolt: SÍ (MCP4 disposa a N1 de E, el N5.MCC.Constraint) 
Cal activar nou MCP: NO (Final d’exploració del camí)
Acord multilateral 1: MCP1 > N5.MCC > N4.MCC > N3.MCC > N2.MCC > MCP1 [1,5,4,3,2,1]

MCP Activat: MCP5 | MCP5.Request = D | MCP5.Mother=MCP3.UCP1 | MCP5.Node=N1
MCC candidat 2 per a resoldre petició: N6.MCC | N6.MCC.Offer=D | N5.MCC.Constraint=B
Constraint resolt: NO | Cal activar nou MCP: SÍ
Acord multilateral 2: N6.MCC > N4.MCC > N3.MCC > N2.MCC > MCP1 [6,4,3,2,1]

MCP Activat: MCP6 | MCP6.Request = B | MCP6.Mother=MCP5.UCP1 | MCP6.Node=N1
MCC candidat per a resoldre petició: N3.MCC | N3.MCC.Offer=B | N3.MCC.Constraint=C
Constraint resolt: SI (N3.MCC ja forma part del camí de l’acord multilateral) 
Cal activar nou MCP: NO (Final d’exploració del camí)
Acord multilateral 2: N3.MCC > N6.MCC > N4.MCC > N3.MCC > N2.MCC > MCP1 [3,6,4,3,2,1]

