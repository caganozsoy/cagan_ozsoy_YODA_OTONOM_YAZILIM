from selenium import webdriver
from selenium.webdriver.common.by import By
from sentence_transformers import SentenceTransformer, util
import time

# 1. Hazırlık: BERT modelini bir kez yükleyelim
print("🤖 Zeka (BERT) yükleniyor...")
model = SentenceTransformer('dbmdz/bert-base-turkish-cased')

# 2. Adım: Safari ile Kitapyurdu'ndan canlı veri çekme
driver = webdriver.Safari()
url = "https://www.kitapyurdu.com/kitap/ugultulu-tepeler/12532.html"

try:
    print("🚀 Safari ile Site-1 (Kitapyurdu) taranıyor...")
    driver.get(url)
    time.sleep(3)

    site1_isim = driver.find_element(By.TAG_NAME, "h1").text
    site1_fiyat_ham = driver.find_element(By.CLASS_NAME, "price__item").text
    site1_fiyat = float(site1_fiyat_ham.strip().replace(",", "."))

    # 3. Adım: Başka bir siteden veri geldiğini varsayalım (Simülasyon)
    site2_isim = "Uğultulu Tepeler - Emily Bronte (Ciltli)"
    site2_fiyat = 310.50  # Örnek fiyat

    # 4. Adım: BERT ile Karşılaştırma
    emb1 = model.encode(site1_isim, convert_to_tensor=True)
    emb2 = model.encode(site2_isim, convert_to_tensor=True)
    benzerlik = util.cos_sim(emb1, emb2).item()

    print(f"\n📊 Karşılaştırma Analizi (Benzerlik: %{benzerlik*100:.2f})")

    if benzerlik > 0.80:
        print("✅ Ürünler Eşleşti! Fiyat Analizi Yapılıyor...")
        if site1_fiyat < site2_fiyat:
            fark = site2_fiyat - site1_fiyat
            print(
                f"💰 AVANTAJ: Site-1 (Kitapyurdu) daha ucuz! Fark: {fark:.2f} TL")
        else:
            fark = site1_fiyat - site2_fiyat
            print(f"💰 AVANTAJ: Site-2 daha ucuz! Fark: {fark:.2f} TL")
    else:
        print("❌ Ürünler eşleşmedi, fiyat karşılaştırması yapılamaz.")

finally:
    driver.quit()
