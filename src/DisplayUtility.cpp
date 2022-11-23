#include "DisplayUtility.h"
#include "M5Atom.h"

int DisplayUtility::color_Error = 0xff0000;
int DisplayUtility::color_Warning = 0xffff00;
int DisplayUtility::color_Moderate = 0x00ff00;
int DisplayUtility::color_Empty = 0x000000;

void DisplayUtility::Fill(int color) {
    for(int i=0; i<25; i++){
    M5.dis.drawpix(i, color);
    }
}

void DisplayUtility::Clear() {
  Fill(color_Empty);
}

void DisplayUtility::Error(){
  Fill(color_Error);

}

void DisplayUtility::Warning(){
  Fill(color_Warning);
}

void DisplayUtility::Moderate(){
  Fill(color_Moderate);
}
