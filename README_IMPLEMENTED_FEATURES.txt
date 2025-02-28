Features implémentées :

Creer une entité qui sera le main char avec déplacement basiques et saut ( 2 à 4 cases )
Editeur ingame avec imgui pour faire les murs et les ennemis
Load / Save sur disque ( juste des triplets position type de cell )
Des ennemis avec des ia à moustache basiques ! ( pas besoin que ca soit joli, ca doit être lisible en revanche )
Support manette
Screenshake - attention à l’empilement !


Non implémentées :

Death ray - laser shot - bresenham
Homing magic missile - avec le moteur de tween simpliste que vous venez de coder exprès. Orientez le “missile” avec atan2.
Pet Drone - fée qui nous suit et tire des projectiles rapides par comportement à l’update, suivis et “blocage” par mur, téléportage si trop eloigné du player
Muzzle fire
Recul - tirer en continue peut nous faire reculer dans le vide
