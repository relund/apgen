#! /usr/bin/perl

# Perl script - REMEMBER TO SAVE THE FILE IN UNIX FORMAT

#-----------------------------------------------------------------------------
# Setup names and paths:
$progname = "../linux/apgen";
$outputdir = "./";
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# Generate multi modal assignment proplems (MMAPs) using method 1, entries in [10,30]
for($run = 0; $run < 100; $run++) {
    for $shape (-60,0,60) {
        for $maxCost (1000,10000) {
            for $dim (5,10) {
                $seed=${run}*21346+1123;
                system("$progname -dim $dim -minEnt 10 -maxEnt 30 -maxCost $maxCost -shape $shape -method 1 -seed ${seed} -out ${outputdir}mmap_d${dim}_e10-30_c0-${maxCost}_m1_s${shape}_$run");
                    print OUT "mmap_d${dim}_e10-30_c0-${maxCost}_m1_s${shape}_$run $dim 10 30 $maxCost $shape 1 $seed\n";
            }
        }
    }
}
#-----------------------------------------------------------------------------



#-----------------------------------------------------------------------------
# Generate APs using method 1, costs in [0,30]
for $dim (5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100) {
   for $run (0,1,2,3,4,5,6,7,8,9) {
      $seed=${run}*21346+1123;
      system("$progname -dim $dim -minEnt 1 -maxEnt 1 -maxCost 30 -shape 0 -method 1 -seed ${seed} -out ${outputdir}ap_d${dim}_c0-30_$run");
   }
}
#-----------------------------------------------------------------------------
