/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

namespace BinaryData
{

//================== README.md ==================
static const unsigned char temp_binary_data_0[] =
"The third and final week of the project has been dedicated to finishing the GUI. Last week I was testing my pitch detection system with simple white text over a black background. The information being outputted by the algorithm has not changed for th"
"e most part, but it is now graphically displayed by the note letter name in large text, whether signal is flat, sharp, or in tune in smaller text, and a large color gradient that transitions from red to green that intuitively represents how far the p"
"itch is from being correct.\n"
"\n"
"Some important refactoring was also done. Before, the pitch detection algorithm and basic GUI were all in one file, which was fine when the layout of the GUI took up so little space, but it now has its own file and is used by PitchDetectionComponent "
"as an object that is fed raw sample data and converts it to useable frequency information.\n"
"\n"
"The ManualTests folder contains two video demos and a text file describing their content.\n";

const char* README_md = (const char*) temp_binary_data_0;


const char* getNamedResource (const char* resourceNameUTF8, int& numBytes);
const char* getNamedResource (const char* resourceNameUTF8, int& numBytes)
{
    unsigned int hash = 0;

    if (resourceNameUTF8 != nullptr)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0x64791dc8:  numBytes = 961; return README_md;
        default: break;
    }

    numBytes = 0;
    return nullptr;
}

const char* namedResourceList[] =
{
    "README_md"
};

const char* originalFilenames[] =
{
    "README.md"
};

const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8)
{
    for (unsigned int i = 0; i < (sizeof (namedResourceList) / sizeof (namedResourceList[0])); ++i)
    {
        if (namedResourceList[i] == resourceNameUTF8)
            return originalFilenames[i];
    }

    return nullptr;
}

}
