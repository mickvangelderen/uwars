/*AI
states
  aggressive
  defensive

aggressive
main target: weak enemy cells
distance: any
support: none
Targetfinding: loop through all (enemy) cells, attack the one with lowest str

smartass
main target: strategic cells
distance: closest
support: cells with low str and close to enemy
Targetfinding: loop through all cells and blobs, if sending blobs can safe your own cell: send reinforcements
			   find hostile and neutral cells that are close by and that do not require massive sacrifices

SV-rating

capacity range = 10 - 256
	score = -(capacity range - 256)^4 >> 16(200)^4) + m   //max score = 78
maxscore = m
score = (m(256 - (

//Str score is negative when attacking and positive when supporting
str range = 0 - 200
	score = (str range)^2 >>9 //max score = 78

GR range = 0 - 15
	score = (GR range)^3 >>5 //max score = 105

SV-rating range = 0 - 261;



u8 scores256[32];
u8 scores16[16];

InitScores(){
	for(u8 x=0; x<256; x++){
		scores256[x] = 


GetScore256(u8 n){
GetScore16(u8 n)



*/