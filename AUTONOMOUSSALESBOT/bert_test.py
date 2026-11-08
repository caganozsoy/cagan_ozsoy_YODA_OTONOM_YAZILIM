from sentence_transformers import SentenceTransformer, util
import torch

# 1. BERT Modelini Yüklüyoruz (Türkçe için en iyisi budur)
# Not: İlk çalışmada bir miktar indirme yapacaktır.
print("🤖 BERT Modeli yükleniyor, lütfen bekleyin...")
model = SentenceTransformer('dbmdz/bert-base-turkish-cased')

# 2. Senaryo: İki farklı e-ticaret sitesinden gelen ürün isimleri
# Biri düz isim, diğeri yayınevi ve yazar bilgisi içeriyor.
site_a_urun = "Uğultulu Tepeler"
site_b_urun = "Uğultulu Tepeler - Emily Bronte - Can Yayınları"

# 3. BERT metinleri 'vektörlere' (sayısal zekaya) dönüştürüyor
print("🧠 İsimler vektörel hale getiriliyor...")
embedding_a = model.encode(site_a_urun, convert_to_tensor=True)
embedding_b = model.encode(site_b_urun, convert_to_tensor=True)

# 4. Cosine Similarity (Kosinüs Benzerliği) ile karşılaştırma yapıyoruz
benzerlik_skoru = util.cos_sim(embedding_a, embedding_b)

# 5. Sonuçları ekrana yazdıralım
print("\n" + "="*30)
print(f"Site A: {site_a_urun}")
print(f"Site B: {site_b_urun}")
print("-" * 30)
print(f"📊 Benzerlik Skoru: %{benzerlik_skoru.item() * 100:.2f}")
print("="*30)

# 6. Karar Mekanizması (Sales Bot Kararı)
threshold = 0.75  # %75 ve üzeri benzerliği 'aynı ürün' kabul ediyoruz
if benzerlik_skoru.item() > threshold:
    print("✅ KARAR: Bu ürünler AYNI. Fiyat karşılaştırması yapılabilir.")
else:
    print("❌ KARAR: Bu ürünler FARKLI. Karşılaştırma yapılamaz.")
