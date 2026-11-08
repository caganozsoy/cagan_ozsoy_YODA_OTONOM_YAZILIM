from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from sentence_transformers import SentenceTransformer, util
import time

# 1. BERT Modeli
print("🤖 Zeka (BERT) yükleniyor...")
model = SentenceTransformer('dbmdz/bert-base-turkish-cased')

# 2. Safari Başlatma
print("🚀 Safari başlatılıyor...")
driver = webdriver.Safari()
driver.maximize_window()

# BURASI KRİTİK: Senin manuel açtığın ve çalışan linki tam olarak buraya koyduk
url = "https://www.kitapyurdu.com/kitap/ugultulu-tepeler/485377.html"

try:
    print(f"📡 Doğru adrese gidiliyor: {url}")
    driver.get(url)

    wait = WebDriverWait(driver, 15)

    # Sayfanın yüklendiğinden emin olalım
    time.sleep(3)

    # 3. Veri Çekme
    print("🔎 Veriler toplanıyor...")

    # Kitap Adı
    urun_adi_el = wait.until(EC.presence_of_element_located(
        (By.CSS_SELECTOR, "h1.pr_header__heading")))
    site1_isim = urun_adi_el.text

    # Fiyat Çekme (Görselindeki sarı butona odaklanıyoruz)
    # Kitapyurdu'nda fiyat bazen 'price__item' bazen de 'div.price-new' içinde olur
    try:
        fiyat_el = driver.find_element(By.CLASS_NAME, "price__item")
        site1_fiyat_ham = fiyat_el.text  # "121,00"

        # Temizleme işlemi
        fiyat_text = site1_fiyat_ham.replace("TL", "").replace(
            ".", "").replace(",", ".").strip()
        site1_fiyat = float(fiyat_text)
    except:
        print("⚠️ Fiyat bulunamadı, ürün satışta olmayabilir.")
        site1_fiyat = 0.0

    print(f"\n✅ BULUNAN: {site1_isim} | Fiyat: {site1_fiyat} TL")

    # 4. BERT Karşılaştırma
    site2_isim = "Uğultulu Tepeler (Can Yayınları)"
    site2_fiyat = 145.50

    emb1 = model.encode(site1_isim, convert_to_tensor=True)
    emb2 = model.encode(site2_isim, convert_to_tensor=True)
    benzerlik = util.cos_sim(emb1, emb2).item()

    print(f"📊 BERT Benzerlik Skoru: %{benzerlik*100:.2f}")

    if benzerlik > 0.75:
        if site1_fiyat > 0:
            print(
                f"💰 SONUÇ: {'Kitapyurdu' if site1_fiyat < site2_fiyat else 'Site-2'} daha ucuz!")
        else:
            print("💰 SONUÇ: Kitapyurdu'nda stok yok, Site-2 tek seçenek.")

except Exception as e:
    print(f"\n❌ HATA: {e}")
    driver.save_screenshot("son_durum.png")

finally:
    time.sleep(5)
    driver.quit()
    print("🏁 İşlem tamamlandı.")
