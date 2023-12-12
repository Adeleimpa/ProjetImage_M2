import cv2
import argparse
import os
from skimage.metrics import structural_similarity as ssim
import imageio.v2 as imageio

def lire_image(chemin_image):
    # Charger l'image
    image = cv2.imread(chemin_image)

    # Vérifier si l'image a été chargée correctement
    if image is None:
        print(f"Impossible de charger l'image à partir de {chemin_image}. Assurez-vous que le chemin est correct.")
        return None

    # Afficher les dimensions de l'image
    hauteur, largeur, canaux = image.shape
    print(f"Dimensions de l'image : {largeur} x {hauteur}, Canaux : {canaux}")

    image = cv2.resize(image, (1024,1024))
    # Afficher l'image (peut nécessiter l'utilisation de cv2.imshow() dans un environnement interactif)
    cv2.imwrite("Image.jpg", image)

    # Attendre jusqu'à ce que la touche ESC (27) soit pressée ou que la fenêtre soit fermée
    key = cv2.waitKeyEx(0)
    if key == 27:  # Touche ESC
        cv2.destroyAllWindows()
        exit()

    cv2.destroyAllWindows()

    return image

def parse_args():
    parser = argparse.ArgumentParser(description='Lire une image avec OpenCV.')
    parser.add_argument('chemin_image', type=str, help='Chemin vers l\'image (jpg, png, etc.).')
    parser.add_argument('sexe', type=str, choices=['h', 'f'], help='Sexe de la personne dans l\'image ("h" pour homme, "f" pour femme).')
    parser.add_argument('nom_dossier', type=str, help='Nom du dossier à créer.')
    return parser.parse_args()

def imageProche(chemin_image, sexe, nom_dossier):
    creer_dossier(nom_dossier)
    image = cv2.imread(chemin_image)
    image_bis = cv2.resize(image, (128,128))
    #print(image.shape)
    chemin_images = "./stylegan/results/pggan_celebahq_gender_editing/"
    if sexe == "f" :
        score_opt = -1
        indice = -1
        for i in range(1000) :
            nom_image = f"{i:03d}_000.jpg"
            chemin_complet = os.path.join(chemin_images, nom_image)
            image_proche = cv2.imread(chemin_complet)
            image_proche_bis = cv2.resize(image_proche, (128,128))
            #if image.shape != image_proche.shape:
                #image_proche_bis = cv2.resize(image, (image_proche.shape[1], image_proche.shape[0]))
            print(i)
            # Comparer les images en utilisant la SSIM
            score_ssim = ssim(image_bis, image_proche_bis, channel_axis=2)
            if score_ssim > score_opt :
                score_opt = score_ssim
                image_opt = image_proche
                indice = i
        cv2.imwrite(f"./animation/{indice:03d}_000.jpg", image)
        for i in range(10):
            nom_image = f"{indice:03d}_{i:03d}.jpg"
            chemin_complet = os.path.join(chemin_images, nom_image)
            image = cv2.imread(chemin_complet)
            cv2.imwrite(f"./animation/{indice:03d}_{i+1:03d}.jpg", image)
    if sexe == "h" :
        score_opt = -1
        indice = -1
        for i in range(1000) :
            nom_image = f"{i:03d}_009.jpg"
            chemin_complet = os.path.join(chemin_images, nom_image)
            image_proche = cv2.imread(chemin_complet)
            image_proche_bis = cv2.resize(image_proche, (128,128))
            #if image.shape != image_proche.shape:
                #image_proche_bis = cv2.resize(image, (image_proche.shape[1], image_proche.shape[0]))
            print(i)
            # Comparer les images en utilisant la SSIM
            score_ssim = ssim(image_bis, image_proche_bis, channel_axis=2)
            if score_ssim > score_opt :
                score_opt = score_ssim
                image_opt = image_proche
                indice = i
        cv2.imwrite(f"./animation/{indice:03d}_000.jpg", image)
        for i in range(10):
            nom_image = f"{indice:03d}_{9-i:03d}.jpg"
            chemin_complet = os.path.join(chemin_images, nom_image)
            image = cv2.imread(chemin_complet)
            cv2.imwrite(f"{nom_dossier}{indice:03d}_{i+1:03d}.jpg", image)

        
        #cv2.imwrite("image_opti.jpg", image_opt)

def creer_dossier(nom_dossier):
    try:
        if not os.path.exists(nom_dossier):
            os.makedirs(nom_dossier)
            print(f"Le dossier '{nom_dossier}' a été créé avec succès.")
        else:
            print(f"Le dossier '{nom_dossier}' existe déjà.")
    except Exception as e:
        print(f"Une erreur s'est produite : {e}")        
            
            
def creer_gif(chemin_dossier, nom_gif):
    images = []

    # Liste les fichiers dans le dossier
    fichiers_images = [f for f in os.listdir(chemin_dossier) if f.lower().endswith(('.png', '.jpg', '.jpeg', '.gif'))]
    fichiers_images.sort()
    for fichier_image in fichiers_images:
        chemin_image = os.path.join(chemin_dossier, fichier_image)
        images.append(imageio.imread(chemin_image))

    # Enregistre le GIF
    chemin_gif = os.path.join(chemin_dossier, nom_gif)
    imageio.mimsave(chemin_gif, images, fps = 10) 


if __name__ == '__main__':
    args = parse_args()
    chemin_image = args.chemin_image
    sexe = args.sexe
    dossier = args.nom_dossier
    image = lire_image(chemin_image)

    

    imageProche(chemin_image, sexe, dossier)

    creer_gif(dossier, "../animation.gif")
