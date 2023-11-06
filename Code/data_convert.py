import os
from PIL import Image

# Chemin du dossier contenant les images d'origine
dossier_source = "/Users/adeleimparato/Documents/Montpellier/UM/Semestre3/Image_Secu_DeepLearning/ProjetImage_M2/Code/img_celeb"

# Chemin du dossier de destination pour les images converties
dossier_destination = "/Users/adeleimparato/Documents/Montpellier/UM/Semestre3/Image_Secu_DeepLearning/ProjetImage_M2/Code/img_converted"

# Création du dossier de destination s'il n'existe pas
if not os.path.exists(dossier_destination):
    os.makedirs(dossier_destination)

# Liste des extensions d'images à traiter (vous pouvez ajouter d'autres formats si nécessaire)
extensions_supportees = (".jpg", ".jpeg", ".png")

# Parcours des fichiers du dossier source
for fichier in os.listdir(dossier_source):
    if fichier.endswith(extensions_supportees):
        chemin_image_source = os.path.join(dossier_source, fichier)

        # Chargement de l'image d'origine
        image = Image.open(chemin_image_source)

        # Redimensionnement en 512x512 pixels
        #image = image.resize((512, 512))

        # Création du nom de fichier pour l'image convertie dans le dossier de destination
        nom_fichier_destination = os.path.splitext(fichier)[0] + ".ppm"
        chemin_image_destination = os.path.join(dossier_destination, nom_fichier_destination)

        # Enregistrement de l'image convertie au format PPM
        image.save(chemin_image_destination, "PPM")

print("Conversion terminée. Les images sont sauvegardées dans le dossier de destination.")
