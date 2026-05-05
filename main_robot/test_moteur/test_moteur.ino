enum dir_type{
  forward,backward
};
#define dir_mode(dir) dir == forward ? 0:1
#define min_delay 600//microseconde
#define max_delay 1000
#define step_pin_M1 14
#define step_pin_M2 12
#define dir_pin_M1 2
#define dir_pin_M2 4
#define PI 3.141592654
#define rayon_roue 7.2/2.0 //7.2cm de diamètre roue
#define micro_step 16
#define dist_entre_roue 19.5
int dir=dir_mode(forward);
int speed=50;//pourcent
#define nb_steps_tour_complet 200//200 pas par tour en mode complet
void setup() {
  Serial.begin(9600);
  pinMode(step_pin_M1,OUTPUT);
  pinMode(step_pin_M2,OUTPUT);
  pinMode(dir_pin_M1,OUTPUT);
  pinMode(dir_pin_M2,OUTPUT);
  delay(2000);
}
void nb_step(int nb_step_m,int M_pin,int M_sens_pin,int sens){
  
}
void nb_step_sync(int nb_step_m,int M1_pin,int M2_pin,int sens1,int sens2){
  digitalWrite(dir_pin_M1,sens1);
  digitalWrite(dir_pin_M2,sens2);
  for(int i=0;i<nb_step_m;i++){
    digitalWrite(M1_pin,HIGH);
    digitalWrite(M2_pin,HIGH);
    delayMicroseconds(min_delay);
    digitalWrite(M1_pin,LOW);
    digitalWrite(M2_pin,LOW);
    delayMicroseconds(min_delay);
  }
  digitalWrite(dir_pin_M1,0);
  digitalWrite(dir_pin_M2,0);
}
int calcul_nb_step_dist(float dist){
  float p_roue=2*PI*rayon_roue;
  float d_step=p_roue/nb_steps_tour_complet;
  return round(dist/d_step);
}
enum types{
  float_,int_
};
enum comande_mode{
  rotation_both,rotation_single,move
};
struct comande{
  enum comande_mode comande_type;
  enum types data_type;
  float val;
};
void slicer(float * tab,String text){
  int id_last=0;
  int id_item=0;
  for(int i=0;i<text.length();i++){
    if((text[i]==',')||(i==text.length()-1)){
      i = i==text.length()-1 ? i+1:i;
      tab[id_item]=text.substring(id_last,i).toFloat();
      //Serial.println(text.substring(id_last,i));
      id_item++;
      id_last=i+1;
    }
  }
}
void read_input(struct comande * comande){
  float tab[3];
  String received=Serial.readString();
  slicer(tab,received);
  // Serial.println("received:");
  // Serial.println(received);
  comande->comande_type=(comande_mode)tab[0];
  comande->data_type=(types)tab[1];
  comande->val=tab[2];
  Serial.println((enum comande_mode)comande->comande_type);
}
// void test_mov(){
//   for(int i=0;i<nb_step_m;i++){
//     digitalWrite(step_pin_M1,HIGH);
//     delayMicroseconds(min_delay);
//     digitalWrite(step_pin_M1,LOW);
//     delayMicroseconds(min_delay);
//   }
// }
void com(){
  struct comande comande;
  if(Serial.available()>0){
    //Serial.println("test3");
    read_input(&comande);
    //Serial.println("test2");
    float p_single=dist_entre_roue*2*PI;
    //Serial.println("test1");
    int sens=comande.val>0 ? 0:1;
    Serial.println("sens:");
    Serial.println(sens);
    comande.val=abs(comande.val);
    
    switch(comande.comande_type){
      case rotation_single:
        Serial.println("single");
        nb_step(calcul_nb_step_dist(p_single*comande.val/360)*micro_step,step_pin_M1,dir_pin_M1,sens);
        break;
      case(rotation_both):
      Serial.println("both");
        nb_step_sync(calcul_nb_step_dist((p_single/2.0)*comande.val/360)*micro_step,step_pin_M1,step_pin_M2,sens,!sens);
        break;
      case(move):
        Serial.println("move");
        nb_step_sync(calcul_nb_step_dist(comande.val)*micro_step,step_pin_M1,step_pin_M2,sens,sens);
        break;
    }
    comande.val=0.0;
    
  }
}
float angle_eff=0.0;
float tourne(float angle,int sens){
  float p_single=dist_entre_roue*2*PI;
  nb_step_sync(calcul_nb_step_dist((p_single/2.0)*angle/360)*micro_step,step_pin_M1,step_pin_M2,sens,!sens);
  return angle;
}
float avance(float distance,int sens){
  float p_single=dist_entre_roue*2*PI;
  nb_step_sync(calcul_nb_step_dist(distance)*micro_step,step_pin_M1,step_pin_M2,sens,sens);
  return distance;
}
int d_aim=50;
int angle_aim=90;
int goal = 4;
void loop() {
  //com();
  for(int i=0;i<goal;i++){
    avance(d_aim,1);
    delay(250);
    tourne(angle_aim,0);
    delay(250);
  }
  goal=0;
}
