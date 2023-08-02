#include "pdf_hummus_make_attachment.hpp"

#include <getopt.h>
#include "string.hpp"
#include <iostream>
int main(int argc, char *argv[])
{
    string project_path = string(getenv("PROJECT_PATH")) * "/";
    string pdf_path = project_path * string("default.pdf");
    string attachment_path = project_path * string("default.tm");
    string out_path = project_path * string("default_out.pdf");
    int c = 0;
    while(EOF != (c = getopt(argc,argv,"t:p:o:")))
    {
        switch(c)
        {
            case 't':
                std::cout << "tmPath " << optarg << std::endl;
                attachment_path = project_path * string(optarg);
                break;
            case 'p':
                std::cout << "pdfPath " << optarg << std::endl;
                pdf_path = project_path* string(optarg);
                break;
            case 'o':
                std::cout << "outPdfPath " << optarg << std::endl;
                out_path = project_path* string(optarg);
                break;
            default:
                std::cout << "bad arg" << std::endl;
                exit(0);
                break;
        }    
    }
    pdf_hummus_make_attachment(pdf_path, attachment_path, out_path);
}