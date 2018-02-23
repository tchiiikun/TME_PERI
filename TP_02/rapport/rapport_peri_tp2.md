###  TP2 : Premier pilote

&nbsp;

## Objectif

L'objectif de la séance est de commander les LEDS et le bouton poussoir (BP) en passant par un pilote installé dans le noyau. 

## Étape 1 : crétation et test d'un noyau

* La fonction "static int __init mon_module_init(void)" est exécutée lorsqu'on insère le module du noyau.

* La fonction "static void __exit mon_module_cleanup(void)" est exécutée lorsqu'on enlève le module du noyau.

	sudo insmod.module.ko
	lsmod
	dmesg 
	> Hello World
	sudo rmmod module
	lsmod
	dmesg 
	> Goodbye World
	
Definitions :
* lsmod   :   Grace au filesystem proc, on peut afficher /proc/modules qui representent les modules courant qui sont chargés dans le
                noyau.
* insmod  :   Insert un module dans le kernel. On peut aussi utiliser modprobe. la seule information qui sera donnée est si le module a été 
                correctement chargé.
* dmesg   :   Affiche à l'écran toutes les informations que le Kernel ring veut bien partager avec les autres rings.
* rmmod   :   Si les options pour unload les modules sont positionnés dans le noyau alors cette commande enlèvera le module du noyau. On peut utiliser
                modprob -r.
	

## Étape 2 : ajout des paramètres au module

	sudo insmod ./module.ko LED=2 BUT=1
	> Hello World
	> LED=2
	> BUT=1
	sudo rmmod ./module.ko LED=2 BUT=1

module_param(LED, int, 0);      // Bind LED en tant qu'int et l'initialise à 0.
MODULE_PARM_DESC(LED, "Nombre de led"); // Ajoute une description.
sudo insmod ./module.ko LED=2           // lors du insmod assigne 2 à LED.

Pour savoir si on a correctement assigné la valeur 2 à LED il suffit de :
printk(KERN_DEBUG "LED=%d !\n", LED);


## Étape 3 : création d'un driver qui ne fait rien mais qui le fait dans le noyau

static int major;                                   // numéro du major.
major = register_chrdev(0, "ledbp", &fops_ledbp);   // register le numéro.
unregister_chrdev(major, "ledbp");                  // unregister le numéro.

Explications : Des noeuds de périphériques (device nodes), sont des resources qu'un noyau a déjà alloué. Ils sont identifés par un numéro majeur et un mineur. Le major désigne un groupe de device qui se "ressemblent" et le mineur un device particulier dans ce groupe. Pour différencier deux périphériques on peut lancer deux fois le même code de driver et lui passer en paramètre (par exemple) le numéro de la LED sur le raspberry pour désigner un device particulier auquel sera associé un minor particulier, mais ils auront le même major et le même code.

Le premier arg désigne le numéro major.

le deuxième est le nom du périphérique qui sera mis dans /proc/devices.

La structure file_operations contient des pointeurs sur les fonctions que le pilote utilise pour implémenter l'interface du pilote au noyau.

mknod prend 3 arguments, le nom dans /dev/, et si on précise l'option "c" alors il crée un character (unbuffered) special file avec l'option "b" il crée un block (buffered) special file.

# Definitions : 
* Block devices (aussi appelé  block special files)  se comportent comme des fichiers ordinaires. C'st un tableau de bytes, la valeur lu à une case du tableau est la valeur qui a été écrite à cette endroit dans le tableau. Cette data peut être caché en mémoire et lu depuis ce cache.
On peut "seek" dans un block devices i.e c'est un devise avec une notion de positionnement qui peut être changé par l'application. Le nom block device vient de la correspondance entre le hardware, typiquement, lire ou écrire un block entier sur un disque.

* Les character devices (aaussi appelés character special files) se comportent comme des pipes. Ecrire et lire est une action immédiate il n'y a pas de regard sur ce que fait le driver avec sa data. 
Apparemment écrire des bytes sur du character devices possède des comportements non défini, comme faire des sons, écrire sur l'écran etc (lire un byte aussi n'est pas recommandé apparemment). Le nom vient du fait que chaque char est individuellement pris en compte.

il faut préciser le numéro du mineur et du majeur.

Pour savoir s'il a été correctement créée il faut regarder soit /dev/ et vérifier avec un grep si notre device est présent. Sinon il faut faire un cat sur /proc/devices.

    la commande dd : convertion et copie de fichiers.
        -bs      : lit et écrire le nombre de bytes précisés
        -count   : copie N block en entrée.
&nbsp;

        $ echo "rien" > /dev/ledbp
        $ dd bs=1 count=1 < /dev/ledbp
        $ dmesg
Ces commandes vont envoyé un message à notre LED qui répondra via dmesg car on a redirigé /dev/ledbp  dans un dd de 1 block vers STDOUT par défaut.   

&nbsp;

        #!/bin/sh
        module=$1
        shift 
        /sbin/insmod ./$module.ko $* || exit 1
        rm -f /dev/$module
        major=$(awk "\$2==\"$module\" {print \$1;exit}" /proc/devices)
        mknod /dev/$module c $major 0
        chmod 666 /dev/$module
        echo "=> Device /dev/$module created with major=$major"
        
Awk est un langage de programmation. la commande awk est structurée ainsi :

&nbsp;
        awk [options] [programme] [fichier]
        où la structure du programme est :
        'motif1 { action1 } motif2 { action2 } …'

&nbsp;

        #!/bin/sh
        module=$1
        /sbin/rmmod $module || exit 1
        rm -f /dev/$module
        echo "=> Device /dev/$module removed"
&nbsp;


## Étape 4 : accès aux GPIO depuis les fonctions du pilote


GPIO (GPIO_BASE 0x20200000) est mappée dans l'espace virtuel du noyau à l'adresse io_addresse et récupérer avec la macro du noyau __io_address(). 

        static int gpio_read(int pin)
        {
            return (gpio_regs->gplev[pin/32] >> (pin % 32))  & 0x1;
        }

## Étape 5 (optionnel) : Usage d'un timer dans le noyau pour faire clignoter la led
Nous avons rajouté le ledblinker dans nos fichiers.

setup_timer
mod_timer
BUG_ON
del_timer
 printk

