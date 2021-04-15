
/*
* disable
! show only
# highlight / debug
% transparent / background 
*/

$fs=0.8;
// Dimensions communes à l'ensemble des boitiers 
//--------- epaisseur du boitier
E=5;
// --------------- epaisseur de l'epoxy
hauteurEpoxy=2;
largeurRainureEpoxy=1.2;
hauteurRainureEpoxy=hauteurEpoxy;

//--------------- hauteur et largeur rainure humidite male
hauteurRainureHumidite=3;
largeurRainureHumidite=2;

// --------------------------passe fil
//rayon petit et gros fils
rayonPetitFil=2.5;
rayonGrosFil=4;
rayonVis=1.6;
// distance trous/bord pour le passe-fils
dTrouBord=2;
//Epaisseur tete vis passe-fils
epaisseurTeteVis=3;
// ------------------------ tube vélo
diametreTube=60;
epaisseurEncoche=E/5;
// taille d'un ecrou M3  : 6.2 x 3.5 (avec un jeu)// a voir 
diametreEcrou=6.2;
epaisseurEcrou=3.5;
// ------------------------ bouton étanches
diametreBoutonEtanche=18;
diametreTrouBoutonEtanche=12;
//--------------------------ecran
longueurEcran=27;
largeurEcran=14;
//distance de la gauche de la carte carte à la gacuhe de l' ecran 
distanceBordX=20.5;
//distance du haut de la carte au haut de l'éecran
distanceBordY=4.5;


////// ------ Dimension pour la carte guidon :
// largeur de la carte electronique
largeurCarte=60+0.8;
//longueur de la carte électronique
L=70+0.5;
//
longueurCarte=L;
//hauteur de la carte electronique
hauteurSurCarte=14;
//hauteur sous la carte electronique
hauteurSousCarte=2;

// pensez à mettre hauteurPasseFil=0 ci-dessous




////// ------ Dimension pour carte puissance 
// largeur de la carte electronique
//largeurCarte=46.2;
//longueur de la carte électronique
//L=81.6;
//longueurCarte=L;
//hauteur de la carte electronique
//hauteurSurCarte=20;
//hauteur sous la carte electronique
// 19 + E
//hauteurSousCarte=19+1;
// pensez à mettre hauteurPasseFil ci-dessous



// Dimensions imposées -------------------------------
//hauteur au dessus de  la carte
HHAUT=epaisseurTeteVis+3*dTrouBord+2*rayonGrosFil+epaisseurEcrou;

largeurInterieure=largeurCarte-2*largeurRainureEpoxy;
//longueur passe fil

longueurPasseFil=hauteurRainureHumidite+diametreEcrou;

//hauteurPasseFil=0 pour la carte guidon
//hauteurPasseFil=epaisseurTeteVis+2*dTrouBord+2*rayonGrosFil+epaisseurEcrou;
hauteurPasseFil=dTrouBord/2;
    
largeurPasseFil=largeurInterieure-2; // On retire 2 mm pour permettre au couvercle de coulisser sans problème.


hauteurTotale=hauteurSurCarte+hauteurSousCarte+hauteurPasseFil+2*E+hauteurEpoxy+2;
hauteurLaterale=hauteurSousCarte+E+hauteurEpoxy+hauteurSurCarte;




// ---------------------------------------------------------------
// Trou pour le passage des câbles boitier puissance
function positionTrouY(i)=dTrouBord+diametreEcrou/2+ (0.8*dTrouBord+diametreEcrou)*i;

function positionGrosTrouY()=largeurPasseFil-dTrouBord-rayonGrosFil; 




// ---------------------------------------------------------------
// ---------------------------------------------------------------

module haricot(largeur,hauteur,epaisseur){
    linear_extrude(height=epaisseur)
    union(){
        circle(d=largeur);
        translate([0,hauteur-largeur])circle(d=largeur);
        translate([-largeur/2,-largeur/2])square(largeur/2);
        translate([-largeur/2,0])square([largeur,hauteur-largeur]);
        }
}
// ---------------------------------------------------------------

module attache(){
    translate([4,4,0])difference(){
        haricot(8,12,2.5);
        haricot(2,6,2.5);
        translate([-5,-4,0])cube([3,12,2.5]);
    }
    
}




// ---------------------------------------------------------------

module trouPasseFilGuidon(){
    
    longueurPasseFil=3*dTrouBord+2*rayonPetitFil+epaisseurEcrou;
    largeurPasseFil=2*dTrouBord+diametreEcrou;
    hauteurPasseFil=2*dTrouBord+diametreEcrou;
    difference(){
        cube([longueurPasseFil,largeurPasseFil,hauteurPasseFil]);
    
        //troufil
        hauteurTrou=hauteurPasseFil;
        
        translate([dTrouBord+rayonPetitFil,largeurPasseFil/2,hauteurTrou/2])cylinder(r=rayonPetitFil,h=hauteurPasseFil+2,center=true);
        
        //trouVis
        tailleTrouVis=2*dTrouBord+epaisseurEcrou;
        translate([2*dTrouBord+2*rayonPetitFil+epaisseurEcrou/2,largeurPasseFil/2,hauteurPasseFil/2])rotate([0,90,0])cylinder(r=rayonVis,h=tailleTrouVis+2,center=true);
        
        //trouEcrou
        largeurTrouEcrou=dTrouBord+diametreEcrou;
        translationXTrou=2*dTrouBord+2*rayonPetitFil+epaisseurEcrou/2;
        translationYTrou=dTrouBord/2+diametreEcrou/2;
        translationZTrou=dTrouBord+diametreEcrou/2;
        
        translate([translationXTrou,translationYTrou,translationZTrou])cube([epaisseurEcrou,largeurTrouEcrou+1,diametreEcrou],center=true);
        
    } 
    
}


module trouBoitierPasseFil(){
    
    //Moyen trous
    translate([longueurPasseFil,positionTrouY(0),dTrouBord+rayonPetitFil+2.5])rotate([0,90,0])cylinder(r=rayonPetitFil+1,h=longueurPasseFil+20,center=true);
    // Petit trous
    for (i=[1:3])  translate([longueurPasseFil,positionTrouY(i),dTrouBord+rayonPetitFil])rotate([0,90,0])cylinder(r=rayonPetitFil,h=longueurPasseFil+20,center=true);
    
    // gros trous
    translate([longueurPasseFil,positionGrosTrouY(),rayonGrosFil+dTrouBord])rotate([0,90,0])cylinder(r=rayonGrosFil,h=longueurPasseFil+20,center=true);    
}


module passeFils(){
difference(){
    //3 mm pour la tete de la vis    
    cube([longueurPasseFil,largeurPasseFil,hauteurPasseFil]);
        
    //petits trous fils   
    
      // passage vis
    for (i=[0:3]){  
      translate([diametreEcrou/2,positionTrouY(i),hauteurPasseFil-epaisseurEcrou])cylinder(r=rayonVis,h=longueurPasseFil+10,center=true);    
    }
    //gros trous fils
    
    translate([diametreEcrou/2,positionGrosTrouY(),hauteurPasseFil-epaisseurEcrou-10])cylinder(r=rayonVis,h=2*longueurPasseFil);
    //trous ecrous petits fils
    for (i=[0:3]){
    // taille d'un ecrou M3 6.2 x 3.5 (avec un jeu)// a voir    
    translate([diametreEcrou/2,positionTrouY(i),hauteurPasseFil-dTrouBord-epaisseurEcrou/2])cube([6.2,6.2,3.5],center=true);  
       }
    //trous écran gros fils
    translate([diametreEcrou/2,positionGrosTrouY(),hauteurPasseFil-dTrouBord-epaisseurEcrou/2])cube([6.2,6.2,3.5],center=true);
    trouBoitierPasseFil();
    }
   
}

// ---------------------------------------------------------------
// module pieceA()
//cote=0 rainure epoxy à gauche cote=1 rainure epoxy a gauche
module pieceA(cote,tol){
    longueurRainurePieceA=longueurCarte+hauteurRainureHumidite*1.5;
    hauteurRainurePieceA=hauteurLaterale+hauteurRainureHumidite;
    largeurRainurehumiditePieceA=largeurRainureHumidite-tol;
    hauteurRainurehumiditePieceA=hauteurRainureHumidite-tol;
    union(){
        difference(){
            cube([longueurCarte,E,hauteurLaterale]);
            if (cote==0)translate([0,E-largeurRainureEpoxy,hauteurSousCarte+E])cube([longueurCarte,largeurRainureEpoxy,hauteurRainureEpoxy]);
            
            if (cote==1)translate([0,0,hauteurSousCarte+E])cube([longueurCarte,largeurRainureEpoxy,hauteurRainureEpoxy]);
            }
            
        translate([-hauteurRainureHumidite/2+longueurCarte/2,E/2,E/2+hauteurRainurePieceA/2]) cube([longueurRainurePieceA,largeurRainurehumiditePieceA,hauteurRainurePieceA],center=true);
        }
}

module pieceB(tol){
       largeurRainureHumiditePieceB=largeurRainureHumidite-tol;
    //EDITJMR : Je ne comprends ce facteur 1.5
       hauteurRainureHumiditePieceB=1.5*hauteurRainureHumidite-tol;
       largeurPieceB=largeurInterieure+2*E;
       largeurRainurePieceB=largeurInterieure+E+largeurRainureHumiditePieceB;
   union (){
       
       cube([longueurCarte+E,largeurPieceB,E]);
       
       translate([-hauteurRainureHumiditePieceB/2,(largeurPieceB)/2,E/2]) cube([hauteurRainureHumiditePieceB,largeurRainurePieceB,largeurRainureHumiditePieceB],center=true);
       } 
}







//dessine=1, on dessine le passefil ; 0 rien

module pieceC(dessine,tol){
    largeurRainurehumiditePieceC=largeurRainureHumidite-tol;
    hauteurRainurehumiditePieceC=hauteurRainureHumidite-tol;
    largeurPieceC=largeurInterieure+2*E;
    hauteurPieceC=hauteurLaterale+hauteurRainureHumidite-tol;
    largeurRainurePieceC=largeurPieceC-E+largeurRainurehumiditePieceC;
    difference(){
        union(){
            cube([E,largeurPieceC,hauteurTotale]);
            //rainure
            #difference(){
                  translate([-hauteurRainurehumiditePieceC,E/2-largeurRainurehumiditePieceC/2,E/2])cube([hauteurRainurehumiditePieceC,largeurRainurePieceC,hauteurPieceC]);
                  translate([-hauteurRainurehumiditePieceC,E/2+largeurRainurehumiditePieceC/2,E/2])cube([hauteurRainurehumiditePieceC,largeurRainurePieceC-2*largeurRainurehumiditePieceC,hauteurPieceC-largeurRainurehumiditePieceC]);
            }
        //passeFil    
        if (dessine==1)translate([-longueurPasseFil,largeurInterieure/2+E-largeurPasseFil/2,hauteurLaterale])passeFils();
        }
        //trouPAsseFil
        if (dessine==1)translate([-longueurPasseFil,largeurInterieure/2+E-largeurPasseFil/2,hauteurLaterale])trouBoitierPasseFil();
    }    
    
}    


// si passeFils=1, on ajoute le passe-fils - inutile pour le couvercle.
module coque(passeFils){
    union(){
             //fond
             cube([L+E,largeurInterieur+2*E,E]);
             //cote gauche avec passe fil
             translate([L+E,0,0])rotate([0,-90,0])cube([hauteurTotale,largeurInterieur+2*E,E]);
             //cote lateral 1
             translate([0,E,0])rotate([90,0,0])cube([L+E,hauteurLaterale,E]);
             //cote lateral 2
             translate([0,largeurCarte+2*E,0])rotate([90,0,0])cube([L+E,hauteurLaterale,E]);
             if (passeFils==1)translate([L-longueurPasseFil,E,hauteurLaterale]) passeFils();
         }
}

// ---------------------------------------------------------------

module boitierPuissance(){

difference(){
    union(){
        pieceB(tol=0);
        pieceA(0,tol=0);
        translate([0,largeurInterieure+E,0])pieceA(1,tol=0);
       
        translate([longueurCarte,0,0])pieceC(1,tol=0);
        // attache
        translate([12,2,2.5])rotate([180,0,-90])attache();
        translate([L,2,2.5])rotate([180,0,-90])attache();
    //
        translate([L,largeurCarte+2*E-5,2.5])rotate([180,0,-90])mirror([12,0,0])attache();
        translate([12,largeurCarte+2*E-5,2.5])rotate([180,0,-90])mirror([12,0,0])attache();
        }
   //tube du vélo : prends 2 mm du boitier.      
   translate([-E,(largeurCarte+2*E)/2,-diametreTube/2+epaisseurEncoche])rotate([0,90,0])cylinder(h=L+2*E,d=diametreTube); 
    }    
}
// ---------------------------------------------------------------

module couverclePuissance(){
    tolerance=-0.6;
    difference(){
        translate([-E,0,0])cube([L+2*E,largeurInterieure+2*E,hauteurTotale]);
        translate([E,0,0])union(){  
                 pieceB(tol=tolerance);
                 // 2 = pas de rainure
                 pieceA(2,tolerance);
                 translate([0,largeurInterieure+E,0])pieceA(2,tolerance);
       
                 translate([longueurCarte,0,0])pieceC(0,tolerance);
            
                } 
       //tube du vélo : prends 2 mm du boitier.      
       translate([-E,(largeurCarte+2*E)/2,-diametreTube/2+epaisseurEncoche])rotate([0,90,0])cylinder(h=L+2*E,d=diametreTube);
       // cubeinterieure         
       translate([E,E,0])cube([L,largeurInterieure*1.001,hauteurTotale-E]);
    }
}    

// --------------
module boitierGuidon(){
tailleConnecteur4point=16;
translateXfil=longueurCarte+E-(rayonPetitFil+2*dTrouBord+epaisseurEcrou);
translateYfil=E+rayonPetitFil+tailleConnecteur4point/2;

difference(){
    union(){
        pieceB(tol=0);
        pieceA(0,tol=0);
        translate([0,largeurInterieure+E,0])pieceA(1,tol=0);
       
        translate([longueurCarte,0,0])pieceC(0,tol=0);
        // attache
        translate([12,2,2.5])rotate([180,0,-90])attache();
        translate([L,2,2.5])rotate([180,0,-90])attache();
    //
        translate([L,largeurCarte+2*E-5,2.5])rotate([180,0,-90])mirror([12,0,0])attache();
        translate([12,largeurCarte+2*E-5,2.5])rotate([180,0,-90])mirror([12,0,0])attache();
        
        // passefil et fil  du 
        
        
        translateXPasseFil=translateXfil-dTrouBord-rayonPetitFil;
        translateYPasseFil=translateYfil-dTrouBord-rayonPetitFil;
        translateZPasseFil=-2*dTrouBord-diametreEcrou;
        translate([translateXPasseFil,translateYPasseFil,translateZPasseFil])trouPasseFilGuidon();
        
        }
   //tube du vélo : prends 2 mm du boitier.      
   translate([-E-(3*dTrouBord+2*rayonPetitFil+epaisseurEcrou+1),(largeurCarte+2*E)/2,-diametreTube/2+epaisseurEncoche])rotate([0,90,0])cylinder(h=L+2*E,d=diametreTube);
   // Trous pour le passage des fils
    
   translate([translateXfil,translateYfil,0])cylinder(r=rayonPetitFil,h=E+20,center=true);
         
   //      
    }    
}
// ---------------------------------------------------------------

module couvercleGuidon(){
    tolerance=-0.6;
    difference(){
        translate([-E,0,0])cube([L+2*E,largeurInterieure+2*E,hauteurTotale]);
        translate([E,0,0])union(){  
                 pieceB(tol=tolerance);
                 // 2 = pas de rainure
                 pieceA(2,tolerance);
                 translate([0,largeurInterieure+E,0])pieceA(2,tolerance);
       
                 translate([longueurCarte,0,0])pieceC(0,tolerance);
            
                } 
       //tube du vélo : prends 2 mm du boitier.      
       translate([-E,(largeurCarte+2*E)/2,-diametreTube/2+epaisseurEncoche])rotate([0,90,0])cylinder(h=L+2*E,d=diametreTube);
       // cubeinterieure         
       translate([E,E,0])cube([L,largeurInterieure+0.1,hauteurTotale-E]);
    
       
                
       // trou pour la fenetre 
       centreTrouX=E+longueurEcran/2+distanceBordX;
       centreTrouY=largeurInterieure+E-largeurEcran/2-distanceBordY;   
      
               
               
               
                
       translate([centreTrouX,centreTrouY,hauteurTotale])cube([longueurEcran,largeurEcran,10],center=true);
       translate([centreTrouX,centreTrouY,hauteurTotale])cube([longueurEcran+10,largeurEcran+10,6],center=true);
         
       // trous pour les boutons
       bouton1X=centreTrouX-diametreBoutonEtanche/2-10;
       bouton1Y=centreTrouY-(largeurEcran+10)/2-diametreBoutonEtanche/2-10;    
       
       
       bouton2X=centreTrouX;
       bouton2Y=centreTrouY-(largeurEcran+10)/2-diametreBoutonEtanche/2-2;       
       
       bouton3X=centreTrouX+diametreBoutonEtanche/2+10;
       bouton3Y=centreTrouY-(largeurEcran+10)/2-diametreBoutonEtanche/2-10; 
       
       translate([bouton1X,bouton1Y,hauteurTotale])cylinder(d=diametreTrouBoutonEtanche,h=10,center=true);
       translate([bouton1X,bouton1Y,hauteurTotale+3])cylinder(d=diametreBoutonEtanche+1,h=10,center=true);
       
       translate([bouton2X,bouton2Y,hauteurTotale])cylinder(d=diametreTrouBoutonEtanche,h=10,center=true);    
       translate([bouton2X,bouton2Y,hauteurTotale+3])cylinder(d=diametreBoutonEtanche+1,h=10,center=true);  
       translate([bouton3X,bouton3Y,hauteurTotale])cylinder(d=diametreTrouBoutonEtanche,h=10,center=true);       
       translate([bouton3X,bouton3Y,hauteurTotale+3])cylinder(d=diametreBoutonEtanche+1,h=10,center=true);       
           
       // joint pour l'écran
       difference(){
           translate([centreTrouX,centreTrouY,hauteurTotale])cube([longueurEcran+7,largeurEcran+7,8],center=true);
           translate([centreTrouX,centreTrouY,hauteurTotale])cube([longueurEcran+3,largeurEcran+3,8],center=true);
       
        }
        
   }  
    
}    



couvercleGuidon();
//boitierGuidon();
//boitierPuissance();
//couverclePuissance();
//pieceA(0,0);
//pieceC(0,0);
