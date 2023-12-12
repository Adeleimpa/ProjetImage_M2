from tkinter import * 
from tkinter.filedialog import *
from tkinter.messagebox import *
from tkinter import ttk

from PIL import ImageTk, Image
import cv2
import os
from skimage.metrics import structural_similarity as ssim
import imageio.v2 as imageio
import numpy as np


global filepath
filepath = ""



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

    return image

#Autoriser l'édition d'image
def alert():
    global fenetre_alerte, label_alerte, bouton_ok
    fenetre_alerte = Toplevel(fenetre)
    fenetre_alerte.title("Alerte personnalisée")
    
    # Définir la taille de la fenêtre
    fenetre_alerte.geometry("400x200")
    
    # Ajouter un label avec le message d'alerte
    message = "Vous pouvez éditer."
    label_alerte = Label(fenetre_alerte, text=message)
    label_alerte.pack(padx=10, pady=10)
    
    # Ajouter un bouton "OK" pour fermer la fenêtre
    bouton_ok = Button(fenetre_alerte, text="OK", command=fermerFenetres)
    bouton_ok.pack(pady=10)

def verifier_selection():
    if value.get() == 1 or value.get() == 2:
        bouton.config(state="active")
    else:
        bouton.config(state="disabled")

def choixImage():
    global filepath, photo, canvas, bouton1, bouton2, bouton, value
    filepath = askopenfilename(title="Ouvrir une image", filetypes=[('png files', '.png'), ('jpeg files', '.jpg'), ('all files', '.*')])
    
    if filepath:  # Vérifie si un fichier a été sélectionné
        #photo = ImageTk.PhotoImage(file=filepath)
        photo = Image.open(filepath)
        newWidth = photo.width//2
        newHeight = photo.height//2
        photo = photo.resize((newWidth,newHeight))
        print(filepath)
        photo = ImageTk.PhotoImage(photo)
        canvas = Canvas(fenetre, width=photo.width(), height=photo.height())
        canvas.create_image(0, 0, anchor=NW, image=photo) 
        canvas.pack()
        value = IntVar() 
        value.set("0")
        bouton1 = Radiobutton(fenetre, text="Cette image représente un homme", variable=value, value="1", command=verifier_selection)
        bouton2 = Radiobutton(fenetre, text="Cette image représente une femme", variable=value, value="2", command=verifier_selection)
        bouton1.pack()
        bouton2.pack()
        bouton=Button(fenetre, text="Valider", command=alert, state = "disabled")
        bouton.pack()

def fermerFenetres():
    canvas.destroy()
    bouton1.destroy()
    bouton2.destroy()
    bouton.destroy()
    fenetre_alerte.destroy()

def creer_dossier(nom_dossier):
    try:
        if not os.path.exists(nom_dossier):
            os.makedirs(nom_dossier)
            print(f"Le dossier '{nom_dossier}' a été créé avec succès.")
        else:
            print(f"Le dossier '{nom_dossier}' existe déjà.")
            showerror("ERREUR !", "Le dossier existe déjà")
    except Exception as e:
        print(f"Une erreur s'est produite : {e}")  

def imageProche():
    global fenetre_prog, fenetre_fin, bouton_fin, entree_gif
    fenetre_prog = Toplevel(fenetre)
    fenetre_prog.title("Progression en cours")
    fenetre_prog.geometry("400x200")
    barre_progression = ttk.Progressbar(fenetre_prog, orient='horizontal', length=300, mode='determinate')
    barre_progression.pack(pady=10)
    global filepath, value, nouveau_chemin
    #creer_dossier(nom_dossier)
    image = cv2.imread(filepath)
    image_bis = cv2.resize(image, (128,128))
    #print(image.shape)
    chemin_images = "./stylegan/results/pggan_celebahq_gender_editing/"
    if value.get() == 2 :
        score_opt = -1
        indice = -1
        for i in range(1000) :
            barre_progression['value'] = i/10
            fenetre_prog.update()
            #fenetre_prog.after(100)
            nom_image = f"{i:03d}_000.jpg"
            chemin_complet = os.path.join(chemin_images, nom_image)
            image_proche = cv2.imread(chemin_complet)
            image_proche_bis = cv2.resize(image_proche, (128,128))
            #if image.shape != image_proche.shape:
                #image_proche_bis = cv2.resize(image, (image_proche.shape[1], image_proche.shape[0]))
            #print(i)
            # Comparer les images en utilisant la SSIM
            score_ssim = ssim(image_bis, image_proche_bis, channel_axis=2)
            if score_ssim > score_opt :
                score_opt = score_ssim
                image_opt = image_proche
                indice = i
        cv2.imwrite(f"{nouveau_chemin}/{indice:03d}_000.jpg", image)
        for i in range(10):
            #barre_progression['value'] = 55 + i * 5
            #fenetre_prog.update()
            #fenetre_prog.after(100)
            nom_image = f"{indice:03d}_{i:03d}.jpg"
            chemin_complet = os.path.join(chemin_images, nom_image)
            image = cv2.imread(chemin_complet)
            cv2.imwrite(f"{nouveau_chemin}/{indice:03d}_{i+1:03d}.jpg", image)
        barre_progression['value'] = 100
        if barre_progression['value'] == 100 :
            fenetre_prog.destroy()
            fenetre_fin = Toplevel(fenetre)
            fenetre_fin.title("Traitement terminé !")
            fenetre_fin.geometry("400x200")
            message_gif = "Choisissez le nom de l'animation"
            label_gif = Label(fenetre_fin, text=message_gif)
            label_gif.pack(padx=10, pady=10)
            entree_gif = Entry(fenetre_fin, width=30)
            entree_gif.pack()
            bouton_fin=Button(fenetre_fin, text="OK", command=validerNomGif)
            bouton_fin.pack()
    if value.get() == 1 :
        score_opt = -1
        indice = -1
        for i in range(1000) :
            barre_progression['value'] = i/10
            fenetre_prog.update()
            #fenetre_prog.after(100)
            nom_image = f"{i:03d}_009.jpg"
            chemin_complet = os.path.join(chemin_images, nom_image)
            image_proche = cv2.imread(chemin_complet)
            image_proche_bis = cv2.resize(image_proche, (128,128))
            #if image.shape != image_proche.shape:
                #image_proche_bis = cv2.resize(image, (image_proche.shape[1], image_proche.shape[0]))
            #print(i)
            # Comparer les images en utilisant la SSIM
            score_ssim = ssim(image_bis, image_proche_bis, channel_axis=2)
            if score_ssim > score_opt :
                score_opt = score_ssim
                image_opt = image_proche
                indice = i
        cv2.imwrite(f"{nouveau_chemin}/{indice:03d}_000.jpg", image)
        for i in range(10):
            #barre_progression['value'] = 55 + i * 5
            #fenetre_prog.update()
            #fenetre_prog.after(100)
            nom_image = f"{indice:03d}_{9-i:03d}.jpg"
            chemin_complet = os.path.join(chemin_images, nom_image)
            image = cv2.imread(chemin_complet)
            cv2.imwrite(f"{nouveau_chemin}/{indice:03d}_{i+1:03d}.jpg", image)
        barre_progression['value'] = 100 
        if barre_progression['value'] == 100 :
            fenetre_prog.destroy()
            fenetre_fin = Toplevel(fenetre)
            fenetre_fin.title("Traitement terminé !")
            fenetre_fin.geometry("400x200")
            message_gif = "Choisissez le nom de l'animation"
            label_gif = Label(fenetre_fin, text=message_gif)
            label_gif.pack(padx=10, pady=10)
            entree_gif = Entry(fenetre_fin, width=30)
            entree_gif.pack()
            bouton_fin=Button(fenetre_fin, text="OK", command=validerNomGif)
            bouton_fin.pack()

def imageProcheBis():
    global fenetre_prog, fenetre_fin, bouton_fin, entree_gif
    fenetre_prog = Toplevel(fenetre)
    fenetre_prog.title("Progression en cours")
    fenetre_prog.geometry("400x200")
    barre_progression = ttk.Progressbar(fenetre_prog, orient='horizontal', length=300, mode='determinate')
    barre_progression.pack(pady=10)
    global filepath, value, nouveau_chemin, indice
    #creer_dossier(nom_dossier)
    image = cv2.imread(filepath)
    image_bis = cv2.resize(image, (128,128))
    #print(image.shape)
    chemin_images = "./stylegan/results/pggan_celebahq_gender_editing/"
    if value.get() == 2 :
        score_opt = -1
        indice = -1
        for i in range(1000) :
            barre_progression['value'] = i/10
            fenetre_prog.update()
            nom_image = f"{i:03d}_000.jpg"
            chemin_complet = os.path.join(chemin_images, nom_image)
            image_proche = cv2.imread(chemin_complet)
            image_proche_bis = cv2.resize(image_proche, (128,128))
            #if image.shape != image_proche.shape:
                #image_proche_bis = cv2.resize(image, (image_proche.shape[1], image_proche.shape[0]))
            #print(i)
            # Comparer les images en utilisant la SSIM
            score_ssim = ssim(image_bis, image_proche_bis, channel_axis=2)
            if score_ssim > score_opt :
                score_opt = score_ssim
                image_opt = image_proche
                indice = i
        cv2.imwrite(f"{nouveau_chemin}/{indice:03d}_000.jpg", image)
        #for i in range(10):
            #barre_progression['value'] = 55 + i * 5
            #fenetre_prog.update()
            #fenetre_prog.after(100)
        nom_image = f"{indice:03d}_009.jpg"
        chemin_complet = os.path.join(chemin_images, nom_image)
        image = cv2.imread(chemin_complet)
        cv2.imwrite(f"{nouveau_chemin}/{indice:03d}_001.jpg", image)
        barre_progression['value'] = 100
        if barre_progression['value'] == 100 :
            fenetre_prog.destroy()
            fenetre_fin = Toplevel(fenetre)
            fenetre_fin.title("Traitement terminé !")
            fenetre_fin.geometry("400x200")
            bouton_fin=Button(fenetre_fin, text="OK", command=validerResultats)
            bouton_fin.pack()
    if value.get() == 1 :
        score_opt = -1
        indice = -1
        for i in range(1000) :
            barre_progression['value'] = i/10
            fenetre_prog.update()
            #fenetre_prog.after(100)
            nom_image = f"{i:03d}_009.jpg"
            chemin_complet = os.path.join(chemin_images, nom_image)
            image_proche = cv2.imread(chemin_complet)
            image_proche_bis = cv2.resize(image_proche, (128,128))
            #if image.shape != image_proche.shape:
                #image_proche_bis = cv2.resize(image, (image_proche.shape[1], image_proche.shape[0]))
            #print(i)
            # Comparer les images en utilisant la SSIM
            score_ssim = ssim(image_bis, image_proche_bis, channel_axis=2)
            if score_ssim > score_opt :
                score_opt = score_ssim
                image_opt = image_proche
                indice = i
        cv2.imwrite(f"{nouveau_chemin}/{indice:03d}_000.jpg", image)
        #for i in range(10):
            #barre_progression['value'] = 55 + i * 5
            #fenetre_prog.update()
            #fenetre_prog.after(100)
        nom_image = f"{indice:03d}_000.jpg"
        chemin_complet = os.path.join(chemin_images, nom_image)
        image = cv2.imread(chemin_complet)
        cv2.imwrite(f"{nouveau_chemin}/{indice:03d}_001.jpg", image)
        barre_progression['value'] = 100
        if barre_progression['value'] == 100 :
            fenetre_prog.destroy()
            fenetre_fin = Toplevel(fenetre)
            fenetre_fin.title("Traitement terminé !")
            fenetre_fin.geometry("400x200")
            bouton_fin=Button(fenetre_fin, text="OK", command=validerResultats)
            bouton_fin.pack()

def validerResultats():
    global indice, nouveau_chemin, photo1, photo2, image1, image2
    image1 = Image.open(f"{nouveau_chemin}/{indice:03d}_000.jpg")
    image2 = Image.open(f"{nouveau_chemin}/{indice:03d}_001.jpg")
    print(f"{nouveau_chemin}/{indice:03d}_000.jpg")
    # Convertir les images en objets PhotoImage
    newWidth1 = image1.width//2
    newHeight1 = image1.height//2
    image1 = image1.resize((newWidth1,newHeight1))
    newWidth2 = image2.width//2
    newHeight2 = image2.height//2
    image2 = image2.resize((newWidth2,newHeight2))
    photo1 = ImageTk.PhotoImage(image1)
    photo2 = ImageTk.PhotoImage(image2)
    # Créer une fenêtre Tkinter
    fenetre_img = Toplevel(fenetre)
    fenetre_img.title("Affichage d'images côte à côte")

    # Créer deux widgets Label pour afficher les images
    label_image1 = Label(fenetre_img, image=photo1)
    label_image1.grid(row=0, column=0, padx=5, pady=5)
    #canvas = Canvas(fenetre_img,width=1024, height=1024)
    #canvas.create_image(0, 0, anchor=NW, image=photo1)
    #canvas.pack()
    label_image2 = Label(fenetre_img, image=photo2)
    label_image2.grid(row=0, column=1, padx=5, pady=5)

def validerNomDossier():
    global dossier, entree, nouveau_chemin, fenetre_dossier
    if(entree.get() != ""):
        nouveau_chemin = os.path.join(dossier, entree.get())
        if os.path.exists(nouveau_chemin):
            showerror("ERREUR !", "Le dossier existe déjà")
        else : 
            creer_dossier(nouveau_chemin)
            fenetre_dossier.destroy()
            imageProche()
    else :
        showerror("ERREUR !", "Nom invalide")

def validerNomDossierBis():
    global dossier, entree, nouveau_chemin, fenetre_dossier
    if(entree.get() != ""):
        nouveau_chemin = os.path.join(dossier, entree.get())
        if os.path.exists(nouveau_chemin):
            showerror("ERREUR !", "Le dossier existe déjà")
        else : 
            creer_dossier(nouveau_chemin)
            fenetre_dossier.destroy()
            imageProcheBis()
    else :
        showerror("ERREUR !", "Nom invalide")

def validerNomGif():
    global dossier, entree_gif, nouveau_chemin, fenetre_dossier
    nom_gif = entree_gif.get() + ".gif"
    if(entree_gif.get() != ""):
        chemin_gif = os.path.join(nouveau_chemin, nom_gif)
        if os.path.exists(chemin_gif):
            showerror("ERREUR !", "Le dossier existe déjà")
        else : 
            fenetre_fin.destroy()
            creer_gif(nouveau_chemin, nom_gif)
            
            #imageProche()
    else :
        showerror("ERREUR !", "Nom invalide")


def creer_gif(chemin_dossier, nom_gif):
    global fenetre_gif

    fenetre_gif = Toplevel(fenetre)
    fenetre_gif.title("Gif")
    
    # Définir la taille de la fenêtre
    fenetre_gif.geometry("1024x1024")

    images = []
    images_gif = []

    # Liste les fichiers dans le dossier
    fichiers_images = [f for f in os.listdir(chemin_dossier) if f.lower().endswith(('.png', '.jpg', '.jpeg', '.gif'))]
    fichiers_images.sort()
    for fichier_image in fichiers_images:
        chemin_image = os.path.join(chemin_dossier, fichier_image)
        image_pil = Image.open(chemin_image)
        newWidth = 1024
        newHeight = 1024
        image_pil = image_pil.resize((newWidth,newHeight))
        images.append(ImageTk.PhotoImage(image_pil))
        images_gif.append(imageio.np.array(image_pil))
    # Créer un widget Canvas pour afficher le GIF
    canvas_gif = Canvas(fenetre_gif, width=images[0].width(), height=images[0].height())
    canvas_gif.pack()

    # Afficher les images du GIF en séquence
    def afficher_image(index=0):
        nonlocal images, canvas_gif
        canvas_gif.create_image(0, 0, anchor=NW, image=images[index])
        fenetre_gif.update()
        index_suivant = (index + 1) % len(images)
        fenetre_gif.after(100, lambda: afficher_image(index_suivant))
    # Lancer la boucle d'affichage
    afficher_image()
    # Enregistrer le GIF
    chemin_gif = os.path.join(chemin_dossier, nom_gif)
    imageio.mimsave(chemin_gif, images_gif, fps=10) 
    print(f"Le GIF '{nom_gif}' a été créé avec succès dans le dossier '{chemin_dossier}'.")


def demander_nom_dossier():
    global fenetre_dossier, entree, dossier
    if filepath != "" :
        dossier = askdirectory(title = "Dans quel dossier souhaitez-vous enregistrer le résultat")
        if dossier :
            fenetre_dossier = Toplevel(fenetre)
            fenetre_dossier.title("Nom du dossier")
    
            # Définir la taille de la fenêtre
            fenetre_dossier.geometry("400x200")
    
            # Ajouter un label avec le message d'alerte
            message_dossier = "Choisissez le nom de votre dossier"
            label_dossier = Label(fenetre_dossier, text=message_dossier)
            label_dossier.pack(padx=10, pady=10)
            # entrée
            entree = Entry(fenetre_dossier, width=30)
            entree.pack()
            # Ajouter un bouton "OK" pour fermer la fenêtre
            bouton_ok = Button(fenetre_dossier, text="OK", command=validerNomDossier)
            bouton_ok.pack(pady=10)
    else :
        showerror("Erreur", "Sélectionnez d'abord une image !")

def demander_nom_dossier_bis():
    global fenetre_dossier, entree, dossier
    if filepath != "" :
        dossier = askdirectory(title = "Dans quel dossier souhaitez-vous enregistrer le résultat")
        if dossier :
            fenetre_dossier = Toplevel(fenetre)
            fenetre_dossier.title("Nom du dossier")
    
            # Définir la taille de la fenêtre
            fenetre_dossier.geometry("400x200")
    
            # Ajouter un label avec le message d'alerte
            message_dossier = "Choisissez le nom de votre dossier"
            label_dossier = Label(fenetre_dossier, text=message_dossier)
            label_dossier.pack(padx=10, pady=10)
            # entrée
            entree = Entry(fenetre_dossier, width=30)
            entree.pack()
            # Ajouter un bouton "OK" pour fermer la fenêtre
            bouton_ok = Button(fenetre_dossier, text="OK", command=validerNomDossierBis)
            bouton_ok.pack(pady=10)
    else :
        showerror("Erreur", "Sélectionnez d'abord une image !")

def aide():
    # Créer une fenêtre d'aide
    fenetre_aide = Toplevel()
    fenetre_aide.title("Aide")

    # Ajouter du contenu à la fenêtre d'aide
    message_aide = (
        "Bienvenue dans l'application Gender Swap !\n"
        "Pour éditer un genre, allez dans Fichier-Ouvrir afin de sélectionner votre image.\n"
        "Une fois l'image choisie, sélectionnez l'édition que vous souhaitez. \n"
        "Pour créer un GIF, allez dans Édition-Animation. \n"
        "Pour ne créer que l'image résultat, allez dans Édition-Image."
    )
    label_aide = Label(fenetre_aide, text=message_aide, padx=20, pady=20)
    label_aide.pack()

    # Ajouter un bouton "Fermer" pour fermer la fenêtre d'aide
    bouton_fermer = Button(fenetre_aide, text="Fermer", command=fenetre_aide.destroy)
    bouton_fermer.pack(pady=10)

fenetre = Tk()

fenetre.attributes('-fullscreen', True)
fenetre.bind('<Escape>',lambda e: fenetre.destroy())

# Ajouter un label en tant que titre principal
label_titre = Label(fenetre, text="SWAP GENDER !", font=("Helvetica", 16))
label_titre.pack()

menubar = Menu(fenetre)

menu1 = Menu(menubar, tearoff=0)
menu1.add_command(label="Ouvrir", command=choixImage)
menu1.add_separator()
menu1.add_command(label="Quitter", command=fenetre.quit)
menubar.add_cascade(label="Fichier", menu=menu1)

menu2 = Menu(menubar, tearoff=0)
menu2.add_command(label="Animation", command=demander_nom_dossier)
menu2.add_command(label="Image", command=demander_nom_dossier_bis)
menubar.add_cascade(label="Éditer", menu=menu2)

menu3 = Menu(menubar, tearoff=0)
menu3.add_command(label="A propos", command=aide)
menubar.add_cascade(label="Aide", menu=menu3)

fenetre.config(menu=menubar)


fenetre.mainloop()
