> File generated with VerFI, from https://github.com/vmarribas/VerMFi

# FAULTS CONFIGURATION FILE

## HYERARCHY

First introduce the number of fault simulations you want to run, how many faults  
to inject in total per simulation and the maximum number of faults in the same cycle  
(substitute XX fields with your choice): 

  N_SIMULS = `3`  
  N_FAULTS = `2`  
  PER_CYCLE = `1`  


A hierarchy of your design is plot below. Several options are given to customize your fault injection:  
* F?: allow fault injection in this module  
* PI/PO?: allow fault injection in the PIs and POs, allowing single wires to be faulted  
* When?: choose clk cycle where faults should be inserted. Ranges are allowed, but only  
greater than or smaller than, like: `>7` or `<7` (cycle will be chosen at random)  
* Type?: SEU, SA0 or SA1 

## NETLIST

Keccak_p200 ->   F?: `0`   When?: `<6`   Type?: `SA0`  
  * Kecc_cntnt ->   F?: `0`   PI/O?: `0`   When?: `>0`   Type?: `SA1`  
  * Kecc_round ->   F?: `1`   PI/O?: `1`   When?: `>0`   Type?: `SEU`  
    * Kecc_round/chi_perm ->   F?: `1`   PI/O?: `1`   When?: `3`   Type?: `SEU`  
    * Kecc_round/theta_perm ->   F?: `0`   PI/O?: `0`   When?: `-1`   Type?: `SEU`  
    * Kecc_round/pi_perm ->   F?: `0`   PI/O?: `0`   When?: `-1`   Type?: `SEU`  
    * Kecc_round/rho_perm ->   F?: `0`   PI/O?: `0`   When?: `-1`   Type?: `SEU`  
    * Kecc_round/iota_perm ->   F?: `0`   PI/O?: `0`   When?: `-1`   Type?: `SEU`  

