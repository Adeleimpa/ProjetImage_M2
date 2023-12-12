import os
import urllib.request

# Changer le répertoire de travail
#os.chdir('/content')

# Cloner le dépôt GitHub
CODE_DIR = 'stylegan'
os.system(f'git clone https://github.com/genforce/interfacegan.git {CODE_DIR}')

# Télécharger les modèles pré-entraînés
os.chdir(f'./{CODE_DIR}/models/pretrain')
urllib.request.urlretrieve('https://www.dropbox.com/s/t74z87pk3cf8ny7/pggan_celebahq.pth?dl=1', 'pggan_celebahq.pth')
urllib.request.urlretrieve('https://www.dropbox.com/s/nmo2g3u0qt7x70m/stylegan_celebahq.pth?dl=1', 'stylegan_celebahq.pth')
urllib.request.urlretrieve('https://www.dropbox.com/s/qyv37eaobnow7fu/stylegan_ffhq.pth?dl=1', 'stylegan_ffhq.pth')