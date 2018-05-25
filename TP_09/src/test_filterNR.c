/* ----------------------- */
/* --- test_filterNR.c --- */
/* ----------------------- */

/*
 * Copyright (c) 2011 Lionel Lacassagne, all rights reserved
 * University Paris Sud 11
 * University Pierre et Marie Curie
 */

#include <stdio.h>
#include <math.h>

#include "def.h"
#include "nrutil.h"
#include "noise.h"
#include "sampleNR.h"
#include "filterNR.h"
#include "my_paths.h"

#define FILENAME_SIZE 128

// ---------------------------------------------------
float32 calc_sigma(float32 S, float32 Sx, float32 Sxx)
// ---------------------------------------------------
{
    return sqrt((S * Sxx - Sx * Sx) / (S * S));
}
// ---------------------------------------
void test_gaussian_noise_computation(void)
// ---------------------------------------
{
    // pour valider que le generateur aleatoire a bien
    // les bons parametres statistiques ...
    
    int i;
    int n = 10000;
    float32 x, sx, sxx;
    
    sx = 0.0f; sxx = 0.0f;
    
    for(i = 0; i < n; i++) {
        x = gaussian_noise(0, 10);
        //printf("%10.4f\n", x);
        sx += x;
        sxx += x*x;
    }
    printf("average = %10.4f sigma = %10.4f\n", sx/n, calc_sigma(n, sx, sxx));
    
    sx = 0.0f; sxx = 0.0f;
    for(i = 0; i < n; i++) {
        x = gaussian_noise(0, 20);
        //printf("%10.4f\n", x);
        sx += x;
        sxx += x*x;
    }
    printf("average = %10.4f sigma = %10.4f\n", sx/n, calc_sigma(n, sx, sxx));
}
// --------------------
void test_kernel1(void)
// --------------------
{
    float32 *K;
    int r = 3;
   
    puts("-------------------");
    puts("--- test_kernel ---");
    puts("-------------------");
    
    K = alloc_f32kernel1D(r);
    init_gaussian_f32kernel1D(K, r, 2.0f);
    display_f32kernel1D(K, r, "%8.3f", "K");
    
    init_average_f32kernel1D(K, r);
    display_f32kernel1D(K, r, "%8.3f", "K");
}
// ----------------------------
void test_generate_sample(void)
// ----------------------------
{
    // generates perfect samples
    
    int size = 32;
    
    uint8 *X;
    
    char *format = "%5d\t";
    char *ext = "txt";
    char *filename;
    char *dst_path = "";
    char complete_filename[FILENAME_SIZE];
    
    X = ui8vector(0, size-1);
    
    filename = "X_constant";
    generate_sample_constant(X, size);
    generate_path_filename_extension(dst_path, filename, ext, complete_filename);
    write_ui8vector(X, 0, size-1, format, complete_filename);
    
    filename = "X_up";
    generate_sample_step_up(X, size, 16);
    generate_path_filename_extension(dst_path, filename, ext, complete_filename);
    write_ui8vector(X, 0, size-1, format, complete_filename);
    
    filename = "X_down";
    generate_sample_step_down(X, size, 16);
    generate_path_filename_extension(dst_path, filename, ext, complete_filename);
    write_ui8vector(X, 0, size-1, format, complete_filename);
    
    filename = "X_up_down";
    generate_sample_step_updown(X, size, 16);
    generate_path_filename_extension(dst_path, filename, ext, complete_filename);
    write_ui8vector(X, 0, size-1, format, complete_filename);
    
    free_ui8vector(X, 0, size-1);
}
// --------------------------------------------------------------------------------
void routine_linear_filter(int size, int amplitude, char *filename, char *dst_path)
// --------------------------------------------------------------------------------
{
    char complete_filename[FILENAME_SIZE];
    char *sep;
    
    char *format = "%5d";
    char *ext = "txt";
    int ndigit;
    
    //int k;
    float32 alpha;
    float32 sigma_noise; // ecart type du bruit
    float32 sigma_gauss; // ecart type du filtre gaussien
    int radius;
    int q;
    //int radius_gauss;
    
    uint8 *X0; // tableau d'echantillons sans bruit
    uint8 *X;  // tableau d'echantillon bruites
    uint8 *Y;  // tableau filtre

    float32 psnr;
    
    // allocation
    X0 = ui8vector(0, size-1);
    X  = ui8vector(0, size-1);
    Y  = ui8vector(0, size-1);
    
    // -------------------------------------------
    // initialisation: choisir parmi les 4 samples
    // -------------------------------------------
    
    generate_sample_constant(X0, size);
    //generate_sample_step_up(X0, size, amplitude);
    //generate_sample_step_down(X0, size, amplitude);
    //generate_sample_step_updown(X0, size, amplitude);
    generate_path_filename_extension(dst_path, filename, ext, complete_filename);
    write_ui8vector(X0, 0, size-1, format, complete_filename);

    // --------------------
    // -- ajout du bruit --
    // --------------------
    
    sep = "_GN_";
    sigma_noise = 10; // bruit
    ndigit = 2;
    gaussian_noise_ui8vector(X0, 0, size-1, (float32) sigma_noise, X);
    psnr = psnr_ui8vector(X0, 0, size-1, X);
    generate_path_filename_sep_k_ndigit_extension(dst_path, filename, sep, sigma_noise, ndigit, ext, complete_filename);
    write_ui8vector(X, 0, size-1, format, complete_filename);
    
    //goto IIR;
   	
	printf("sigma_noise\t");
	printf("sigma_gauss\t");
	printf("q\t");
	printf("radius\t");
	printf("alpha\t");
	
	printf("FIR_F32\t");
	printf("FIR_I16\t");
	printf("FIR_Q16\t");
	printf("G\t");
	
	printf("IIR_F32\t");
	printf("IIR_Q16\t");
	printf("IIR_Q32\n");

  
	for (alpha = 0.4f; alpha <= 0.9f; alpha+=0.2f) {
		for (q = 8; q <= 13; q+=2) {						
			for (radius = 4; radius <= 11; radius+=4) {
				for (sigma_gauss = 0.5f; sigma_gauss <= 1.6f; sigma_gauss+=0.5f) {
					    
					printf("%6.2f\t", sigma_noise);
					printf("%6.2f\t", sigma_gauss);
					printf("%d\t", q);
					printf("%d\t", radius);
					printf("%6.2f\t", alpha);
					
					// ----------------------
					// -- filtre moyenneur --
					// ----------------------
				FIR:   
					// -- calcul flottant
					fir_average_f32(X, size, radius, Y);
					psnr = psnr_ui8vector(X0, 0, size-1, Y);
					printf("%6.2f\t", psnr);
					sep = "_F32_";
					generate_path_filename_sep_k_ndigit_extension(dst_path, filename, sep, sigma_noise, ndigit, ext, complete_filename);
					write_ui8vector(Y, 0, size-1, format, complete_filename);
					
					// -- calcul entier avec division
					fir_average_i16(X, size, radius, Y);
					psnr = psnr_ui8vector(X0, 0, size-1, Y);
					printf("%6.2f\t", psnr);
					sep = "_I16_";
					generate_path_filename_sep_k_ndigit_extension(dst_path, filename, sep, sigma_noise, ndigit, ext, complete_filename);
					write_ui8vector(Y, 0, size-1, format, complete_filename);
					
					// -- calcul entier avec fraction equivalente en Q16
					fir_average_q16(X, size, radius, q, Y); // sur q bits
					psnr = psnr_ui8vector(X0, 0, size-1, Y);
					printf("%6.2f\t", psnr);
					sep = "_Q16_";
					generate_path_filename_sep_k_ndigit_extension(dst_path, filename, sep, sigma_noise, ndigit, ext, complete_filename);
					write_ui8vector(Y, 0, size-1, format, complete_filename);
					  
					// ---------------------
					// -- filtre gaussien --
					// ---------------------

					ndigit = 2; // codage sur 2 chiffre de la valeur de sigma dans le nom du fichier
					
					fir_gauss_f32(X, size, sigma_gauss, Y);
					psnr = psnr_ui8vector(X0, 0, size-1, Y);
					printf("%6.2f\t", psnr);
					sep = "_G_";
					generate_path_filename_sep_k_ndigit_extension(dst_path, filename, sep, 10*sigma_noise, ndigit, ext, complete_filename);
					write_ui8vector(Y, 0, size-1, format, complete_filename);
					
					
					// ---------------------
					// -- filtre recursif --
					// ---------------------
				 IIR:
				 
					iir_f32(X, size, alpha, Y); // sur 8 bits
					psnr = psnr_ui8vector(X0, 0, size-1, Y);
					printf("%6.2f\t", psnr);
					sep = "_IIRF_";
					generate_path_filename_sep_k_ndigit_extension(dst_path, filename, sep, sigma_noise, ndigit, ext, complete_filename);
					write_ui8vector(Y, 0, size-1, format, complete_filename);
					
					// -- calcul en Q
					iir_q16(X, size, alpha, q, Y); // sur q bits
					psnr = psnr_ui8vector(X0, 0, size-1, Y);
					printf("%6.2f\t", psnr);
					sep = "_IIRI16_";
					generate_path_filename_sep_k_ndigit_extension(dst_path, filename, sep, sigma_noise, ndigit, ext, complete_filename);
					write_ui8vector(Y, 0, size-1, format, complete_filename);
					
					// -- calcul en Q
					iir_q32(X, size, alpha, q, Y); // sur q bits
					psnr = psnr_ui8vector(X0, 0, size-1, Y);
					printf("%6.2f\n", psnr);
					sep = "_IIRQ32_";
					generate_path_filename_sep_k_ndigit_extension(dst_path, filename, sep, sigma_noise, ndigit, ext, complete_filename);
					write_ui8vector(Y, 0, size-1, format, complete_filename);
				}
			}
		}
	}
    // ------------------------------
    // -- liberation de la memoire --
    // ------------------------------
    
    free_ui8vector(X0, 0, size-1);
    free_ui8vector(X,  0, size-1);
    free_ui8vector(Y,  0, size-1);
}
// --------------------------
void test_linear_filter(void)
// --------------------------
{
    int size = 40;
    int amplitude = 40;
    char *filename;
    char *dst_path = "";
    
    puts("----------------");
    puts("--- test_fir ---");
    puts("----------------");
    
    // smooth filter for gaussian noise
    filename = "X1"; routine_linear_filter(size, amplitude, filename, dst_path);
}
// ----------------
void test_iir(void)
// ----------------
{
    int size = 40;
    int amplitude = 40;
    char *filename;
    char *dst_path = "";
    
    puts("----------------");
    puts("--- test_iir ---");
    puts("----------------");
    
    // smooth filter for gaussian noise
    //filename = "X1"; routine_linear_filter(size, amplitude, filename, dst_path);
    
}
/* ----------------------------------------------------------------------------------------------------------- */
void routine_median_filter(char *src_path, char *noise_path, char *filename0, char *filename, char *denoise_path)
/* ----------------------------------------------------------------------------------------------------------- */
{   /*
    char complete_filename[1024];
    char *sep;
    
    int radius;
    int k;
    int ndigit = 1;
    long i0, i1, j0, j1;
    uint8 **X0, **X, **Y;
    float32 p;
    
    // chargement de l'image sans bruit dans X0 et de l'image bruitee dans X
    // Y doit contenir l'image traitee
    generate_path_filename_extension(src_path, filename0, "pgm", complete_filename);
    X0 = LoadPGM_ui8matrix(complete_filename, &i0, &i1, &j0, &j1);
    generate_path_filename_extension(noise_path, filename, "pgm", complete_filename);
    X  = LoadPGM_ui8matrix(complete_filename, &i0, &i1, &j0, &j1);
    Y = ui8matrix(i0, i1, j0, j1);
    
    // -------------------
    // -- median kernel --
    // -------------------
    sep = "_M_";
    
    radius = 1; // parametre du bloc de code
    k = radius;
    median_ui8matrix(X, i0, i1, j0, j1, radius, Y);
    p = psnr_ui8matrix(X0, i0, i1, j0, j1, Y);
    generate_path_filename_sep_k_ndigit_extension(denoise_path, filename, sep, k, ndigit, "pgm", complete_filename);
    printf("%s -> PSNR = %8.2f db\n", complete_filename, p);
    SavePGM_ui8matrix(Y, i0, i1, j0, j1, complete_filename);
    */
    // ajouter ici d'autre instances du bloc de code avec d'autres valeurs de radius
} 
/* ----------------------- */
void test_median_filter(void)
/* ----------------------- */
{
    //char *src_path;
    //char *noise_path;
    //char *denoise_path;
    
}
/* ------------------------------------------ */
int test_filterNR(int argc, const char * argv[])
/* ------------------------------------------ */
{
    puts("=====================");
    puts("=== test_filterNR ===");
    puts("=====================");
    
    // tests unitaires
    //test_kernel();
    //test_average_kernel();
    //test_gaussian_kernel();
    
    //test_gaussian_noise_computation();
    
    //test_generate_sample(); // generation de samples non bruites
    
    // -- fonctions de bruitage
    //test_generate_gaussian_noise(); // OK 2016
    //test_generate_impulse_noise(); // OK 2016
    
    // -- fonctions de debruitage
    test_linear_filter();
    //test_median_filter();
    
    return 0;
}