// Khởi động 2021.01.14
// Thiết kế để làm phim Sóng Tròn
// Phim mặc định có khổ 4096 x 2160 điểm ảnh
// Lệnh Ví Dụ: <bềRộngẢnh> <bềCaoẢnh> <sốHạt> <sốLượngHoạtHình> <sốTậpHợpMàu> <sốLặpLạiMàu>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "PNG.h"

#define kDUNG 1
#define kSAI  0


#define kBE_CAO_ANH__MAC_DINH   2160
#define kBE_RONG_ANH__MAC_DINH  4096
#define kSO_LUONG__HOAT_HINH_MAC_DINH 256


// ---- XOÁY
typedef struct {
   float x;
   float y;
   float suc;
   float tanSo;
   float giam;
} Xoay;


void tinhAnh( float *mangKetQua, unsigned short beRongAnh, unsigned short beCaoAnh, float trungTamX, float trungTamY, float buoc );
//unsigned int mauCuaGiaTri( unsigned char giaTri );

#define kHAI_PI 6.28318f

// ---- bảng màu
void bangMau( float giaTri, float *mangMau ) {

   mangMau[0] = 0.5f + 0.5f*cosf( kHAI_PI*(giaTri + 0.263f) );
   mangMau[1] = 0.5f + 0.5f*cosf( kHAI_PI*(giaTri + 0.416f) );
   mangMau[2] = 0.5f + 0.5f*cosf( kHAI_PI*(giaTri + 0.557f) );
   mangMau[3] = 1.0f;
}


void tinhAnhKinhVanHoa( unsigned char *anh, unsigned int beRongAnh, unsigned int beCaoAnh, unsigned int soHoatHinh, unsigned char dungDu ) {

   float buoc = 2.0f/(float)beCaoAnh;
   float nuaBeCaoAnh = (float)(beCaoAnh >> 1);
   float y = -1.0f;
   
   float thoiGian = soHoatHinh*0.1f;
   
   // ---- chỉnh hệ số phóng to và màu (cho nó đa dạng hơn)
   float heSoPhongTo = 1.3f;// + 0.2f*sinf( kHAI_PI*soHoatHinh*0.005f );   // hằng số 1.5 và 2 cũng đẹp
   // giá trị nhỏ nhơ giảm chi tiết
   
   // ---- chia số lượng xoáy
   unsigned short hangAnh = 0;
   unsigned int diaChiAnh = 0;
   
   /*
    • Giữ góc được tính trong phạm vi 0 đến 2π
    • Xong đơn vị hóa bời 2π cho 'đơn vị hóa' góc cho tô màu
    */
   
   while( hangAnh < beCaoAnh ) {
      float x = -(float)(beRongAnh >> 1)/nuaBeCaoAnh;
      
      // ---- cho biết đang tính tới đâu
      if( !(hangAnh % 32) )
         printf( "  Hàng ảnh: %d/%d\n", hangAnh, beCaoAnh );

      unsigned short cotAnh = 0;
      
      while( cotAnh < beRongAnh ) {
         
         // ---- bán kính từ trung tâm ảnh
         float banKinh0 = sqrt( x*x + y*y );
         float mangMauTong[4] = {0.0f, 0.0f, 0.0f, 1.0f};
         
         // ---- đặt bằng tọa độ điểm này trong ảnh
         float phongToX = x;
         float phongToY = y;
         
         unsigned char chiSo = 0;
         while( chiSo < 4 ) {
            // ---- khi lặp lại, chỉnh phóng to c
            phongToX = heSoPhongTo*phongToX;
            phongToY = heSoPhongTo*phongToY;
            
            float u = 0.0f;
            float v = 0.0f;
            
            if( dungDu ) {
               u = remainderf( phongToX, 1.0f );
               v = remainderf( phongToY, 1.0f );
            }
            else {
               u = phongToX - floorf( phongToX );
               v = phongToY - floorf( phongToY );
               u -= 0.5f;
               v -= 0.5f;
            }

            float banKinh = sqrt( u*u + v*v );
            banKinh *= expf( -banKinh0 );
            
            // ---- tính màu
            float mangMau[4] = {0.0f, 0.0f, 0.0f, 0.0f};
            bangMau( banKinh0 + thoiGian*0.05f, mangMau );
            
            banKinh = sinf( banKinh*8.0f + thoiGian )*0.125f;
            
            // -----
            if( banKinh < 0.0f )
               banKinh = -banKinh;
            
            // ----
            if( banKinh != 0.0f )
               banKinh = powf( 0.01/banKinh, 1.2f );
            else
               banKinh = 0.0f;

            // ---- cộng màu
            mangMauTong[0] += mangMau[0]*banKinh;
            mangMauTong[1] += mangMau[1]*banKinh;
            mangMauTong[2] += mangMau[2]*banKinh;
            
            chiSo++;
         }

         unsigned short mauDo = mangMauTong[0]*255.0f;
         unsigned short mauLuc = mangMauTong[1]*255.0f;
         unsigned short mauXanh = mangMauTong[2]*255.0f;
         
         if( mauDo > 255 )
            mauDo = 255;
         if( mauLuc > 255 )
            mauLuc = 255;
         if( mauXanh > 255 )
            mauXanh = 255;
         
         // ---- đơn vị hóa 'góc kết qủa'
         anh[diaChiAnh] = mauDo;
         diaChiAnh++;
         anh[diaChiAnh] = mauLuc;
         diaChiAnh++;
         anh[diaChiAnh] = mauXanh;
         diaChiAnh++;
         anh[diaChiAnh] = 0xff;
         diaChiAnh++;

         cotAnh++;
         x += buoc;
      }
      
      hangAnh++;
      y += buoc;
   }
}



int main( int argc, char **argv ) {
   
   // ---- phân tích tha số
   unsigned short beRongAnh = kBE_RONG_ANH__MAC_DINH;
   unsigned short beCaoAnh = kBE_CAO_ANH__MAC_DINH;
   unsigned int hatNgauNhien = 0;
   unsigned short hoatHinhToiDa = kSO_LUONG__HOAT_HINH_MAC_DINH;
   unsigned char dungDu = kSAI;  // dùng giải thuật dư
   
   // ---- khổ ảnh
   if( argc > 2) {
      sscanf( argv[1], "%hi", &beRongAnh );
      sscanf( argv[2], "%hi", &beCaoAnh );
      
      // ----
      if( beRongAnh < 512 )
         beRongAnh = 512;
      
      if( beCaoAnh < 256 )
         beCaoAnh = 256;
   }
   else {
      printf( "Lệnh Ví Dụ: <bềRộngẢnh> <bềCaoẢnh> <sốHạt> <sốLượngHoạtHình> <giải dùng dư>\n" );
      printf( "    Ví Dụ:./SongXoay 4096 2160 500 0\n" );
      printf( "          ./songXoay 1024 512 250 1\n" );
      exit(0);
   }
   
   // ---- những tham số không yêu cầu

   // ---- số lượng hoạt hình tối đa
   if( argc > 3 ) {
      sscanf( argv[3], "%hu", &hoatHinhToiDa );
      
      // ---- số lượng hoạt hình tối thiểu
      if( hoatHinhToiDa < 16 ) {
         hoatHinhToiDa = 16;
      }
   }
   
   // ---- dùng dư
   if( argc > 4 ) {
      sscanf( argv[4], "%hhu", &dungDu );
   }

   
   // ----
   printf( " Khổ: %d x %d\n", beRongAnh, beCaoAnh );
   printf( " Số hoạt hình: %d\n", hoatHinhToiDa );
   printf( " Dùng dư: %d\n", dungDu );

   //      exit(0);
   // ==== tô ảnh từ kết qủa
   unsigned char *anh = malloc( beRongAnh*beCaoAnh << 2 );
   if( anh ) {
      
      unsigned int soHoatHinh = 0;
      while( soHoatHinh < hoatHinhToiDa ) {
         
         // ==== quét qua phạm vi ảnh - v
         tinhAnhKinhVanHoa( anh, beRongAnh, beCaoAnh, soHoatHinh, dungDu );

         // ==== Lưu Ảnh
         char tenTep[255];
         sprintf( tenTep, "KinhVanHoa_%04d.png", soHoatHinh );
         luuAnhPNG( tenTep, anh, beRongAnh, beCaoAnh, kPNG_BGRO );
         printf( "Dã lưu tệp: %s\n", tenTep );
         
         // ---- ảnh kế tiếp
         soHoatHinh++;
      }

      free( anh );
   }
   else {
      printf( "Vấn đề giành trí nhớ!\n" );
      exit(0);
   }

   return 1;
}

