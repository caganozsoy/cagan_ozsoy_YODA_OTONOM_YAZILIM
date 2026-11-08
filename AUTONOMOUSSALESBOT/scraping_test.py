from selenium import webdriver
from selenium.webdriver.common.by import By
import time

# Safari için kurulum çok daha basittir, ek driver indirmeye gerek duymaz
driver = webdriver.Safari()

url = "https://www.kitapyurdu.com/kitap/hunlar-ve-tanrinin-kilici-atilla/398321.html"

try:
    print("🚀 Safari başlatılıyor...")
    driver.get(url)

    # JavaScript'in yüklenmesi için bekliyoruz
    time.sleep(3)

    # Kitap adı
    urun_adi = driver.find_element(By.TAG_NAME, "h1").text

    # Fiyat (Class ismi senin ekran görüntündeki gibi)
    fiyat_elementi = driver.find_element(By.CLASS_NAME, "price__item")

    print(f"\n✅ Kitap Adı: {urun_adi}")
    print(f"💰 Bulunan Fiyat: {fiyat_elementi.text}")

except Exception as e:
    print(f"❌ Hata oluştu: {e}")

finally:
    driver.quit()
    print("\n🏁 Tarayıcı kapatıldı.")
