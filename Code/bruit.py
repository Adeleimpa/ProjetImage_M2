from PIL import Image
import numpy as np
import random
import os

def add_noise(image, noise_percentage=0.05):

    largeur, hauteur = image.size

    # Créer une matrice de bruit avec la même forme que l'image
    noise = np.random.randint(0, 256, size=(hauteur, largeur, 3), dtype=np.uint8)

    # Créer une nouvelle image PIL à partir de la matrice de bruit
    noisy_image_pil = Image.fromarray(noise)

    for x in range(0, largeur):
        for y in range(0, hauteur):
            couleurBase = image.getpixel((x,y))
            couleurBruitee = noisy_image_pil.getpixel((x,y))
            melangeCouleur = ( int ((1-noise_percentage) * couleurBase[0] + noise_percentage * couleurBruitee[0]), int ((1-noise_percentage) * couleurBase[1] + noise_percentage * couleurBruitee[1]), int ((1-noise_percentage) * couleurBase[2] + noise_percentage * couleurBruitee[2]))
            noisy_image_pil.putpixel((x,y), melangeCouleur)

    return noisy_image_pil

def generate_noise():
    val = random.randint(0,100)
    print(val)
    modulo = val % 5
    print(modulo)
    if modulo < 3 :
        val = val - modulo 
    else :
        val = val + 5 - modulo
    return val/100

def main():
    # Spécifiez le chemin du dossier contenant les images
    dossier_images = "./basePPM/"
    dossier_images_bruitees = "./images_bruitees/"

    # Obtenez la liste des fichiers dans le dossier
    fichiers_images = os.listdir(dossier_images)

    
    # Parcourez chaque fichier dans le dossier
    for fichier in fichiers_images:
        # Vérifiez si le fichier est un fichier image (vous pouvez ajuster cela selon les types de fichiers que vous avez)
        if fichier.endswith(".ppm"):
            # Construisez le chemin complet du fichier
            chemin_image = os.path.join(dossier_images, fichier)

            # Ouvrez l'image
            original_image = Image.open(chemin_image)

            # Appliquez l'opération souhaitée sur l'image ici (par exemple, ajoutez du bruit)
            # Remplacez cette ligne par l'opération spécifique que vous souhaitez appliquer
            noise_percentage = generate_noise()  # bruit aleatoire
            image_bruitee = add_noise(original_image, noise_percentage)

            # Obtenez le nom du fichier sans l'extension
            nom_fichier, extension = os.path.splitext(fichier)

            # Créez le nouveau nom de fichier pour l'image bruitée
            nouveau_nom_fichier = f"{nom_fichier}_bruitee{extension}"

            # Enregistrez l'image bruitée dans le dossier "images_bruitees"
            chemin_sortie_bruitee = os.path.join(dossier_images_bruitees, nouveau_nom_fichier)
            image_bruitee.save(chemin_sortie_bruitee)

            # Fermez les images
            original_image.close()
            image_bruitee.close()

if __name__ == "__main__":
    main()